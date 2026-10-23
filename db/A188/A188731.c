/*
 * A188731  Decimal expansion of (5+sqrt(41))/4.
 * https://oeis.org/A188731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n‑th decimal digit of (5+sqrt(41))/4, with a(1)=2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_DIGITS 20000            /* enough for n up to about 10000 */
#define BASE 1000000000U            /* 1e9, used for the big integer base */

/* ---------- Big integer : base 1e9 ------------------------------------ */
typedef struct {
    uint32_t d[2048];
    size_t len;
} BigInt;

/* initialise to zero */
static void bi_zero(BigInt *b) { b->len = 0; }

/* set to a small value (0…BASE‑1) */
static void bi_set_small(BigInt *b, uint32_t v) {
    if (v == 0) { b->len = 0; return; }
    b->len = 1; b->d[0] = v;
}

/* copy */
static void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    for (size_t i = 0; i < src->len; ++i) dst->d[i] = src->d[i];
}

/* multiply by a small integer (1…100) */
static void bi_mul_small(BigInt *b, uint32_t m) {
    if (m == 0) { b->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * m + carry;
        b->d[i] = prod % BASE;
        carry   = prod / BASE;
    }
    while (carry) {
        b->d[b->len++] = carry % BASE;
        carry /= BASE;
    }
}

/* add a small integer (0…9) */
static void bi_add_small(BigInt *b, uint32_t a) {
    uint64_t carry = a;
    size_t i = 0;
    while (carry) {
        if (i == b->len) { b->d[b->len++] = 0; }
        uint64_t sum = (uint64_t)b->d[i] + carry;
        b->d[i] = sum % BASE;
        carry   = sum / BASE;
        ++i;
    }
}

/* compare : returns <0 if a<b, 0 if a==b, >0 if a>b */
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}

/* subtract b from a (a>=b) */
static void bi_sub(BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - (int64_t)(i < b->len ? b->d[i] : 0) - borrow;
        if (diff < 0) { diff += BASE; borrow = 1; } else borrow = 0;
        a->d[i] = (uint32_t)diff;
    }
    while (a->len && a->d[a->len - 1] == 0) --a->len;
}

/* ---------- Static state ---------------------------------------------- */
static uint8_t sqrt_frac[MAX_DIGITS];      /* digits of sqrt(41) after the decimal point */
static uint8_t x_digits[MAX_DIGITS];       /* digits of (sqrt(41)-3)/4 after the decimal point */
static size_t sqrt_frac_len = 0;           /* number of computed fractional digits of sqrt(41) */
static uint32_t remX = 3;                 /* remainder for dividing (sqrt41-3) by 4 */
static bool sqrt_initialized = false;

static BigInt root;   /* the current partial square root (integer + fractional digits) */
static BigInt R;      /* remainder during the square‑root algorithm */

/* --------------------------------------------------------------------- */
/* Compute at least 'needed' digits of sqrt(41) and of (sqrt41-3)/4      */
static void ensure_sqrt_digits(size_t needed) {
    /* Initialise sqrt algorithm once */
    if (!sqrt_initialized) {
        bi_zero(&root);
        bi_set_small(&R, 41);          /* start with the number 41          */
        /* integer part: find d s.t. d*d <= 41 */
        int int_digit = 0;
        for (int d = 9; d >= 0; --d) {
            if ((uint32_t)d * (uint32_t)d <= 41) { int_digit = d; break; }
        }
        bi_set_small(&root, (uint32_t)int_digit);             /* root = 6           */
        bi_set_small(&R, 41 - (uint32_t)int_digit * (uint32_t)int_digit); /* R = 5              */
        sqrt_initialized = true;
        remX = 3;                                           /* integer part of sqrt41-3 is 3 */
        sqrt_frac_len = 0;                                  /* no fractional digits yet   */
    }

    while (sqrt_frac_len < needed) {
        /* bring down two zeros: R *= 100 */
        bi_mul_small(&R, 100);

        /* p = root * 20  */
        BigInt p; bi_copy(&p, &root); bi_mul_small(&p, 20);

        /* find the largest d (0…9) with (p + d) * d <= R */
        int chosen_d = 0;
        for (int d = 9; d >= 0; --d) {
            BigInt cand; bi_copy(&cand, &p); bi_add_small(&cand, (uint32_t)d);
            bi_mul_small(&cand, (uint32_t)d);            /* cand = (p + d)*d */
            if (bi_cmp(&cand, &R) <= 0) { chosen_d = d; break; }
        }

        /* update root: root = root*10 + d */
        bi_mul_small(&root, 10);
        bi_add_small(&root, (uint32_t)chosen_d);

        /* update remainder */
        BigInt cand; bi_copy(&cand, &p); bi_add_small(&cand, (uint32_t)chosen_d);
        bi_mul_small(&cand, (uint32_t)chosen_d);
        bi_sub(&R, &cand);

        /* store the sqrt fraction digit */
        sqrt_frac[sqrt_frac_len] = (uint8_t)chosen_d;

        /* compute the corresponding digit of (sqrt41-3)/4 */
        uint32_t temp = remX * 10u + (uint32_t)chosen_d;
        uint8_t digit = (uint8_t)(temp / 4u);
        remX = temp % 4u;
        x_digits[sqrt_frac_len] = digit;

        ++sqrt_frac_len;
    }
}

/* --------------------------------------------------------------------- */
/* The required OEIS sequence function */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;               /* integer part */
    if (n <= 0) return (seq_t)0;               /* out of range; not used in tests */

    size_t pos = (size_t)(n - 2);              /* 0‑based index in the fractional part */
    if (pos >= MAX_DIGITS) return (seq_t)0;    /* safety guard */

    ensure_sqrt_digits(pos + 1);                /* generate digits up to required position */

    return (seq_t)x_digits[pos];
}
