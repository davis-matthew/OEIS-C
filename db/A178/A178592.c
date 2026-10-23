/*
 * A178592  Decimal expansion of sqrt(165).
 * https://oeis.org/A178592
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  cons, nonn, changed
 * Formula:   decimal expansion of sqrt(165); a(2) = 1 (first integer digit),
 *            a(3) = 2 (second integer digit), a(4) = 8 (first decimal digit),
 *            etc
 * Range:     u8 n <= 8524, u16 n <= 8524, u32 n <= 8524, u64 n <= 8524,
 *            u128 n <= 8524, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Simple big integer in decimal base 10 (least‑significant digit first). */
typedef struct {
    size_t len;
    uint8_t *d;          /* digits 0–9, d[0] is least significant. */
} BigInt;

/* ---------- BigInt helpers ---------- */

static void big_init(BigInt *a) { a->len = 0; a->d = NULL; }

static void big_free(BigInt *a) { free(a->d); a->len = 0; a->d = NULL; }

static void big_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    dst->d = (uint8_t *)realloc(dst->d, dst->len);
    if (dst->len) memcpy(dst->d, src->d, dst->len);
}

static void big_set_uint(BigInt *a, uint64_t v) {
    a->len = 0;
    if (v == 0) { a->d = NULL; return; }
    a->d = (uint8_t *)malloc(20); /* enough for up to 64‑bit number. */
    while (v) {
        a->d[a->len++] = v % 10;
        v /= 10;
    }
}

static int big_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i > 0; --i) {
        uint8_t da = a->d[i - 1];
        uint8_t db = b->d[i - 1];
        if (da != db) return (da < db) ? -1 : 1;
    }
    return 0;
}

/* dest = src * m   (m <= 100)  */
static void big_mul_small(BigInt *dest, const BigInt *src, uint32_t m) {
    if (m == 0) { big_free(dest); return; }
    size_t src_len = src->len;
    dest->d = (uint8_t *)realloc(dest->d, src_len + 3);
    uint32_t carry = 0;
    size_t i;
    for (i = 0; i < src_len; ++i) {
        uint32_t prod = src->d[i] * m + carry;
        dest->d[i] = prod % 10;
        carry = prod / 10;
    }
    while (carry) {
        dest->d[i++] = carry % 10;
        carry /= 10;
    }
    dest->len = i;
}

/* dest = src + add   (add <= 81)  */
static void big_add_small(BigInt *dest, const BigInt *src, uint32_t add) {
    size_t src_len = src->len;
    dest->d = (uint8_t *)realloc(dest->d, src_len + 1);
    uint32_t carry = add;
    size_t i;
    for (i = 0; i < src_len; ++i) {
        uint32_t sum = src->d[i] + carry;
        dest->d[i] = sum % 10;
        carry = sum / 10;
    }
    while (carry) {
        dest->d[i++] = carry % 10;
        carry /= 10;
    }
    dest->len = i;
}

/* dest = a - b   (a >= b) */
static void big_sub(BigInt *dest, const BigInt *a, const BigInt *b) {
    dest->d = (uint8_t *)realloc(dest->d, a->len);
    dest->len = a->len;
    int borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int ai = a->d[i];
        int bi = (i < b->len) ? b->d[i] : 0;
        int diff = ai - bi - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        dest->d[i] = (uint8_t)diff;
    }
    while (dest->len > 0 && dest->d[dest->len - 1] == 0)
        --dest->len;
}

/* ---------- Global state for the sequence ---------- */

static uint8_t *dec_digits = NULL;    /* digits after the two integer digits */
static size_t digits_len = 0;         /* total digits stored (integer + decimals) */
static size_t digits_cap = 0;

static BigInt *root_big = NULL;       /* current root value (integer part + decimals) */
static BigInt *rem_big = NULL;        /* current remainder */
static int initialized = 0;

/* ensure enough capacity for decimal digits array */
static void ensure_digits_cap(size_t needed) {
    if (digits_cap < needed) {
        size_t newcap = digits_cap ? digits_cap * 2 : 8;
        while (newcap < needed) newcap *= 2;
        dec_digits = (uint8_t *)realloc(dec_digits, newcap);
        digits_cap = newcap;
    }
}

/* ---------- Initialization ---------- */

static void init_sequence(void) {
    if (initialized) return;
    /* integer part of sqrt(165) */
    root_big = (BigInt *)malloc(sizeof(BigInt));
    rem_big  = (BigInt *)malloc(sizeof(BigInt));
    big_init(root_big);
    big_init(rem_big);

    big_set_uint(root_big, 12);          /* floor sqrt(165) */
    big_set_uint(rem_big, 21);           /* 165 - 12^2 */

    /* store the two integer digits: 1 and 2 */
    digits_cap = 8;
    dec_digits = (uint8_t *)malloc(digits_cap);
    dec_digits[0] = 1;   /* a(2) */
    dec_digits[1] = 2;   /* a(3) */
    digits_len = 2;

    initialized = 1;
}

/* ---------- Core algorithm: generate digits on demand ---------- */

static void generate_next_decimal_digit(void) {
    /* Temporary big integers reused every call */
    BigInt mul100, divisor, cand, tempRoot;
    big_init(&mul100);
    big_init(&divisor);
    big_init(&cand);
    big_init(&tempRoot);

    /* tmp100 = remainder * 100 */
    big_mul_small(&mul100, rem_big, 100);

    /* divisor = root * 20 */
    big_mul_small(&divisor, root_big, 20);

    uint32_t chosen_d = 0;
    for (uint32_t dd = 9; dd <= 9; --dd) {  /* dd is unsigned, stop after 0 */
        /* cand = divisor * dd */
        big_mul_small(&cand, &divisor, dd);
        /* cand += dd*dd */
        big_add_small(&cand, &cand, dd * dd);
        if (big_cmp(&cand, &mul100) <= 0) {
            chosen_d = dd;
            break;
        }
    }

    /* remainder = mul100 - cand */
    big_sub(rem_big, &mul100, &cand);

    /* root = root * 10 + chosen_d */
    big_mul_small(&tempRoot, root_big, 10);   /* root * 10 */
    big_add_small(root_big, &tempRoot, chosen_d);

    /* store the digit */
    ensure_digits_cap(digits_len + 1);
    dec_digits[digits_len++] = (uint8_t)chosen_d;

    /* Clean up temporaries */
    big_free(&mul100);
    big_free(&divisor);
    big_free(&cand);
    big_free(&tempRoot);
}

/* ---------- The required function ---------- */

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;  /* undefined for n < 2 */
    init_sequence();

    /* Integer digits are stored in dec_digits[0] (a(2)) and [1] (a(3)) */
    if (n <= 3) return (seq_t)dec_digits[n - 2];

    size_t decimal_needed = (size_t)(n - 3); /* 0‑based index into decimal part */

    while ((digits_len - 2) <= decimal_needed) {
        generate_next_decimal_digit();
    }

    return (seq_t)dec_digits[n - 2];
}
