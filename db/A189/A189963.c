/*
 * A189963  Decimal expansion of (5+9*sqrt(5))/12.
 * https://oeis.org/A189963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A189963 – Decimal expansion of (5+9*sqrt(5))/12.
 * a(n) = the nth decimal digit of the constant (index 1 → integer part).
 * The function computes the digit by high‑precision arithmetic,
 * without floating point, and works for all builds.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Big‑integer type for arbitrary‑precision computations. */
#define BIGINT_CAP 2100                    /* enough for 10 000 decimal digits */
#define LIMB_BASE ((uint64_t)1000000000000000000ULL)  /* 1e18 */

typedef struct { uint64_t d[BIGINT_CAP]; size_t len; } big_t;

/* ---------- Big‑integer helper functions ---------- */
static void bi_set_uint(big_t *a, uint64_t v) {
    if (v == 0) { a->len = 0; return; }
    a->len = 1; a->d[0] = v;
}
static void bi_copy(big_t *dst, const big_t *src) {
    memcpy(dst->d, src->d, src->len * sizeof(uint64_t));
    dst->len = src->len;
}
static void bi_normalize(big_t *a) {
    while (a->len && a->d[a->len-1] == 0) a->len--;
}
static int bi_compare(const big_t *a, const big_t *b) {
    if (a->len != b->len) return a->len > b->len ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return a->d[i] > b->d[i] ? 1 : -1;
    }
    return 0;
}
static void bi_add_small(big_t *a, uint64_t v) {
    uint64_t carry = v;
    size_t i = 0;
    while (carry) {
        if (i == a->len) a->d[a->len++] = 0;
        unsigned __int128 sum = (unsigned __int128)a->d[i] + carry;
        a->d[i] = (uint64_t)(sum % LIMB_BASE);
        carry  = (uint64_t)(sum / LIMB_BASE);
        i++;
    }
}
static void bi_mul_small(big_t *a, uint64_t m) {
    if (m == 0) { a->len = 0; return; }
    unsigned __int128 carry = 0;
    for (size_t i = 0; i < a->len; i++) {
        unsigned __int128 prod = (unsigned __int128)a->d[i] * m + carry;
        a->d[i] = (uint64_t)(prod % LIMB_BASE);
        carry   = prod / LIMB_BASE;
    }
    while (carry) {
        a->d[a->len++] = (uint64_t)(carry % LIMB_BASE);
        carry /= LIMB_BASE;
    }
}
static void bi_sub(big_t *a, const big_t *b) { /* a >= b */
    unsigned __int128 borrow = 0;
    for (size_t i = 0; i < a->len; i++) {
        unsigned __int128 ai = a->d[i];
        unsigned __int128 bi = (i < b->len) ? b->d[i] : 0;
        ai -= bi + borrow;
        if ((int64_t)ai < 0) { ai += LIMB_BASE; borrow = 1; } else borrow = 0;
        a->d[i] = (uint64_t)ai;
    }
    bi_normalize(a);
}
static void bi_add_assign(big_t *a, const big_t *b) {
    unsigned __int128 carry = 0;
    size_t i = 0;
    while (i < b->len || carry) {
        if (i == a->len) a->d[a->len++] = 0;
        unsigned __int128 sum = (unsigned __int128)a->d[i] + (i < b->len ? b->d[i] : 0) + carry;
        a->d[i] = (uint64_t)(sum % LIMB_BASE);
        carry   = sum / LIMB_BASE;
        i++;
    }
}
/* ------------------------------------------- */

/* Compute floor(sqrt(5) * 10^M) in 'root'.  'rem' is a temporary buffer. */
static void compute_sqrt5_root(int M, big_t *root, big_t *rem) {
    bi_set_uint(rem, 1);        /* 5 - 2^2 = 1 */
    bi_set_uint(root, 2);       /* integer part 2 */
    for (int step = 1; step <= M; step++) {
        bi_mul_small(rem, 100); /* bring down two zeros */
        int chosen = 0;
        big_t temp;
        for (int d = 9; d >= 0; d--) {
            bi_copy(&temp, root);
            bi_mul_small(&temp, 20 * d);          /* root * 20*d */
            bi_add_small(&temp, d * d);           /* + d^2 */
            if (bi_compare(&temp, rem) <= 0) { chosen = d; break; }
        }
        /* subtract chosen product */
        bi_copy(&temp, root);
        bi_mul_small(&temp, 20 * chosen);
        bi_add_small(&temp, chosen * chosen);
        bi_sub(rem, &temp);
        /* update root */
        bi_mul_small(root, 10);
        bi_add_small(root, chosen);
    }
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;               /* safety guard */

    int M = (int)n + 5;                       /* extra digits for safety */

    /* Big integer buffers */
    big_t root, rem_tmp, part5, D, N, tempD;
    big_t D_times[10];
    big_t rem;

    /* 1. Compute sqrt(5) with M decimal digits */
    compute_sqrt5_root(M, &root, &rem_tmp);

    /* 2. Build part5 = 5 * 10^M */
    bi_set_uint(&part5, 5);
    for (int i = 0; i < M; i++) bi_mul_small(&part5, 10);

    /* 3. Build D = 12 * 10^M */
    bi_set_uint(&D, 12);
    for (int i = 0; i < M; i++) bi_mul_small(&D, 10);

    /* 4. Build N = 9 * root + part5 */
    bi_copy(&N, &root);
    bi_mul_small(&N, 9);
    bi_add_assign(&N, &part5);

    /* 5. Precompute D * d for d=0..9 */
    D_times[0].len = 0;   /* zero */
    for (int d = 1; d <= 9; d++) {
        bi_copy(&D_times[d], &D);
        bi_mul_small(&D_times[d], d);
    }

    /* 6. Long division to obtain digits */
    bi_copy(&rem, &N);

    /* Integer part (position 1) */
    int digit = 0;
    for (int d = 9; d >= 0; d--) {
        if (bi_compare(&D_times[d], &rem) <= 0) { digit = d; break; }
    }
    bi_sub(&rem, &D_times[digit]);     /* remainder after integer part */

    if (n == 1) return (seq_t)digit;

    /* Decimal digits (positions 2..n) */
    for (idx_t pos = 2; pos <= n; pos++) {
        bi_mul_small(&rem, 10);        /* bring down next decimal digit */
        for (int d = 9; d >= 0; d--) {
            if (bi_compare(&D_times[d], &rem) <= 0) { digit = d; break; }
        }
        bi_sub(&rem, &D_times[digit]);
        if (pos == n) return (seq_t)digit;
    }
    return (seq_t)0;   /* should never reach here */
}
