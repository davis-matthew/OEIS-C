/*
 * A176109  Decimal expansion of (15+sqrt(255))/10.
 * https://oeis.org/A176109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(1) = 3; for n >= 2, a(n) = floor((15 + sqrt(255)) * 10^{n-2})
 *            mod 10
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Basic bigint implementation in base 10 (least-significant digit first). */
#define MAX_DIGITS 21000

typedef struct {
    size_t len;              /* number of used digits */
    uint8_t d[MAX_DIGITS];   /* digits, 0–9, d[0] is least significant */
} BigInt;

static void bi_zero(BigInt *b) { b->len = 0; }
static void bi_trim(BigInt *b) {
    while (b->len > 1 && b->d[b->len - 1] == 0) b->len--;
}
static void bi_from_uint(BigInt *b, uint32_t val) {
    bi_zero(b);
    if (val == 0) {
        b->len = 1;
        b->d[0] = 0;
        return;
    }
    while (val) {
        b->d[b->len++] = val % 10;
        val /= 10;
    }
}
static void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len);
}
static void bi_mul_small(BigInt *b, uint32_t m) {
    if (m == 0) {
        bi_from_uint(b, 0);
        return;
    }
    uint32_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint32_t prod = b->d[i] * m + carry;
        b->d[i] = prod % 10;
        carry = prod / 10;
    }
    while (carry) {
        b->d[b->len++] = carry % 10;
        carry /= 10;
    }
}
static void bi_add_small(BigInt *b, uint32_t a) {
    uint32_t carry = a;
    size_t i = 0;
    while (carry && i < b->len) {
        uint32_t sum = b->d[i] + carry;
        b->d[i] = sum % 10;
        carry = sum / 10;
        ++i;
    }
    while (carry) {
        b->d[b->len++] = carry % 10;
        carry /= 10;
    }
}
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] > b->d[i]) return 1;
        if (a->d[i] < b->d[i]) return -1;
    }
    return 0;
}
static void bi_sub(BigInt *a, const BigInt *b) {
    /* assume a >= b */
    int borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int diff = a->d[i] - borrow - (i < b->len ? b->d[i] : 0);
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = diff;
    }
    bi_trim(a);
}

/* Global state for the digit generation algorithm */
static int initialized = 0;
static BigInt P;          /* current integer part of sqrt(255) times 10^k */
static BigInt R;          /* current remainder in the algorithm */
static size_t seen_k = 0; /* number of digits already stored in sqrt_dec */
static uint8_t *sqrt_dec = NULL;
static size_t sqrt_dec_cap = 0;

/* Ensure storage for sqrt_dec up to size 'need' */
static void ensure_sqrt_dec_capacity(size_t need) {
    if (need <= sqrt_dec_cap) return;
    size_t new_cap = sqrt_dec_cap ? sqrt_dec_cap * 2 : 128;
    while (new_cap < need) new_cap *= 2;
    uint8_t *tmp = (uint8_t *)realloc(sqrt_dec, new_cap);
    if (!tmp) exit(1);        /* allocation failure */
    sqrt_dec = tmp;
    sqrt_dec_cap = new_cap;
}

/* Generate decimal digits of sqrt(255) up to index 'k_needed' (inclusive). */
static void generate_digits_until(size_t k_needed) {
    /* k_needed is 0‑based index into sqrt_dec */
    while (seen_k <= k_needed) {
        /* R ← R * 100 */
        bi_mul_small(&R, 100);

        /* C ← P * 20 */
        BigInt C;
        bi_copy(&C, &P);
        bi_mul_small(&C, 20);

        /* Find largest d (0 ≤ d ≤ 9) with (C + d) * d ≤ R */
        int d;
        BigInt temp, left;
        for (d = 9; d >= 0; --d) {
            bi_copy(&temp, &C);
            bi_add_small(&temp, d);
            bi_copy(&left, &temp);
            bi_mul_small(&left, d);
            if (bi_cmp(&left, &R) <= 0) break;
        }

        /* Update P: P ← P * 10 + d */
        bi_mul_small(&P, 10);
        bi_add_small(&P, d);

        /* Update R: R ← R – left */
        bi_sub(&R, &left);

        /* Store digit */
        ensure_sqrt_dec_capacity(seen_k + 1);
        sqrt_dec[seen_k++] = (uint8_t)d;
    }
}

static void init_globals(void) {
    if (initialized) return;
    bi_from_uint(&P, 15);
    bi_from_uint(&R, 30);
    initialized = 1;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)0;

    /* Convert to 0‑based index into sqrt_dec */
    size_t k_needed = (size_t)(n - 3);
    init_globals();
    generate_digits_until(k_needed);
    return (seq_t)sqrt_dec[k_needed];
}
