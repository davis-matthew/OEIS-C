/*
 * A179021  Decimal expansion of sqrt(231)/4.
 * https://oeis.org/A179021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the nth decimal digit of √231 / 4 (1‑indexed)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system */

/* Big‑integer helpers (base 10^9) */
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

typedef struct {
    uint32_t *d;   /* little‑endian limbs in base BASE */
    size_t    len;
} BigInt;

#define BASE 1000000000u

static void init_bigint(BigInt *x) { x->d = NULL; x->len = 0; }

static void free_bigint(BigInt *x) { free(x->d); x->d = NULL; x->len = 0; }

static void normalize(BigInt *x) {
    while (x->len && x->d[x->len-1] == 0) x->len--;
}

static void copy_bigint(const BigInt *src, BigInt *dst) {
    dst->len = src->len;
    if (src->len) {
        dst->d = (uint32_t*)malloc(src->len * sizeof(uint32_t));
        memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    } else
        dst->d = NULL;
}

static void mul_small_inplace(BigInt *x, uint32_t m) {
    if (m == 0 || !x->len) { free_bigint(x); return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i]     = (uint32_t)(prod % BASE);
        carry       = prod / BASE;
    }
    while (carry) {
        x->d = (uint32_t*)realloc(x->d, (x->len + 1) * sizeof(uint32_t));
        x->d[x->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

static void add_uint32_inplace(BigInt *x, uint32_t m) {
    uint64_t carry = m;
    size_t i = 0;
    while (carry && i < x->len) {
        uint64_t sum = (uint64_t)x->d[i] + carry;
        x->d[i]     = (uint32_t)(sum % BASE);
        carry       = sum / BASE;
        ++i;
    }
    if (carry) {
        x->d = (uint32_t*)realloc(x->d, (x->len + 1) * sizeof(uint32_t));
        x->d[x->len++] = (uint32_t)carry;
    }
}

static void sub_assign(BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    size_t i = 0;
    for (; i < b->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - b->d[i] - borrow;
        if (diff < 0) { diff += BASE; borrow = 1; }
        else          { borrow = 0; }
        a->d[i] = (uint32_t)diff;
    }
    while (borrow && i < a->len) {
        int64_t diff = (int64_t)a->d[i] - borrow;
        if (diff < 0) { diff += BASE; borrow = 1; }
        else          { borrow = 0; }
        a->d[i] = (uint32_t)diff;
        ++i;
    }
    normalize(a);
}

static int compare_big(const BigInt *a, const BigInt *b) {
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] > b->d[i]) return 1;
        if (a->d[i] < b->d[i]) return -1;
    }
    return 0;
}

/* Extract one decimal digit of the square root.  The caller must
   multiply rem by 100 before calling this function.  After the
   call, rem holds the new remainder.  The root value is updated
   in p. */
static void next_sqrt_digit(BigInt *rem, BigInt *p, uint32_t *digit) {
    /* Compute candidate for this digit. */
    BigInt p20; init_bigint(&p20);
    copy_bigint(p, &p20);
    mul_small_inplace(&p20, 20);

    for (int x = 9; x >= 0; --x) {
        BigInt temp; init_bigint(&temp);
        copy_bigint(&p20, &temp);
        mul_small_inplace(&temp, (uint32_t)x);

        add_uint32_inplace(&temp, (uint32_t)(x * x));

        if (compare_big(&temp, rem) <= 0) {
            /* Accept this digit. */
            sub_assign(rem, &temp);
            mul_small_inplace(p, 10);
            add_uint32_inplace(p, (uint32_t)x);
            *digit = (uint32_t)x;

            free_bigint(&p20);
            free_bigint(&temp);
            return;
        }
        free_bigint(&temp);
    }
    /* Should never be reached. */
    *digit = 0;
    free_bigint(&p20);
}

/* ---------- Memoised extraction ---------- */
static seq_t *digits = NULL;          /* digits of √231/4 */
static size_t digits_len = 0;
static size_t digits_cap = 0;

static void ensure_digits_capacity(size_t needed) {
    if (needed <= digits_cap) return;
    size_t new_cap = digits_cap ? digits_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = seq_alloc_array(new_cap);
    if (digits_len) {
        memmove(new_arr, digits, digits_len * sizeof(seq_t));
        seq_free_array(digits);
    }
    digits = new_arr;
    digits_cap = new_cap;
}

static bool initialized = false;
static BigInt rem, p;
static int div_rem = 0;              /* remainder for division by 4 */

/* Initialise from the integer part of √231 = 15. */
static void init_sequence(void) {
    init_bigint(&rem);
    init_bigint(&p);

    /* Groups for integer part: 2 | 31 */
    const uint32_t int_groups[2] = {2, 31};
    for (size_t i = 0; i < 2; ++i) {
        mul_small_inplace(&rem, 100);
        add_uint32_inplace(&rem, int_groups[i]);

        uint32_t dummy;
        next_sqrt_digit(&rem, &p, &dummy);   /* obtains next integer digit */
    }

    /* p now holds the integer part 15.  p may consist of one limb. */
    uint64_t p_val = 0;
    for (size_t i = 0; i < p.len; ++i)
        p_val = p_val * BASE + p.d[i];

    seq_t int_digit = (seq_t)(p_val / 4);
    seq_t rem_digit = (seq_t)(p_val % 4);

    ensure_digits_capacity(1);
    digits[0] = int_digit;
    digits_len = 1;
    div_rem = (int)rem_digit;

    initialized = true;
}

/* Compute digits up to position n (1‑indexed). */
static void fill_digits_until(idx_t n) {
    if (!initialized) init_sequence();
    while (digits_len < (size_t)n) {
        /* Bring down next pair of zeros for the next decimal digit. */
        mul_small_inplace(&rem, 100);

        uint32_t sqrt_digit;
        next_sqrt_digit(&rem, &p, &sqrt_digit);

        /* Perform long division by 4 on the expanded number. */
        int cur = div_rem * 10 + (int)sqrt_digit;
        int qdigit = cur / 4;
        div_rem = cur % 4;

        ensure_digits_capacity(digits_len + 1);
        digits[digits_len++] = (seq_t)qdigit;
    }
}

/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    fill_digits_until(n);
    return digits[n-1];
}
