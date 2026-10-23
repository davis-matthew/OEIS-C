/*
 * A194856  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {r}, {2r}, ..., {nr}, where r=-sqrt(5).
 * https://oeis.org/A194856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A194856 – Triangular array with permutation of (1,…,r) given by
 * increasing ordering of fractional parts {r}, {2r}, …, {nr} with r = -√5.
 *
 * The code implements the following algorithm:
 *   1. Find row r such that T_{r-1} < n ≤ T_r (T_k = k(k+1)/2).
 *   2. Generate the array 1..r and their associated floor values
 *      floor(k·√5) = ⌊√(5·k²)⌋ using pure integer arithmetic.
 *   3. Sort the array in descending order of the fractional part of k·√5
 *      by comparing the floor values and the indices without any
 *      floating‑point operations.
 *   4. Return the element at position n – T_{r-1} – 1 inside that row.
 *
 * No floating‑point math and no <math.h> are used; only integer
 * arithmetic (up to 128 bits) and standard library functions
 * (<stdlib.h>, <stdint.h>).
 */

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

/* integer square root of a 128 bit unsigned integer, floor value */
static inline uint64_t isqrt128(__uint128_t v)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 63;           /* 2^63 */
    while (bit > 0) {
        uint64_t rbit = r | bit;
        __uint128_t sq = (__uint128_t)rbit * rbit;
        if (sq <= v)
            r = rbit;
        bit >>= 1;
    }
    return r;
}

/* floor(k·√5) computed as floor(√(5·k²)) */
static inline uint64_t floor_sqrt5(uint64_t k)
{
    __uint128_t prod = (__uint128_t)5 * k * k;   /* 5 k² fits in 128 bits for our use */
    return isqrt128(prod);
}

/* structure used during sorting */
typedef struct {
    uint64_t val;   /* position k (1 ≤ k ≤ r) */
    uint64_t fld;   /* floor(k·√5) */
} item_t;

/* comparator: x should come before y if fractional part of x·√5 > that of y·√5 */
static int cmp_desc(const void *a, const void *b)
{
    const item_t *x = (const item_t *)a;
    const item_t *y = (const item_t *)b;
    int64_t D = (int64_t)x->val - (int64_t)y->val;
    int64_t delta = (int64_t)x->fld - (int64_t)y->fld;   /* ≠ 0 for distinct indices */

    if (D > 0) {
        /* 5·D² vs delta²  (note delta > 0 because floor values increase with k) */
        __int128 left  = (__int128)5 * D * D;      /* 5·D² */
        __int128 right = (__int128)delta * delta;  /* delta² */
        return (left > right) ? -1 : 1;
    } else if (D < 0) {
        /* |delta| > sqrt(5)·(-D)  →  delta² > 5·(-D)²  */
        __int128 left  = (__int128)5 * (-D) * (-D); /* 5·(-D)² */
        __int128 right = (__int128)delta * delta;   /* delta² (negative) */
        return (right > left) ? -1 : 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* --- find row r such that T_{r‑1} < n ≤ T_r --- */
    __uint128_t t = (__uint128_t)8 * n + 1;
    uint64_t s = isqrt128(t);            /* floor(sqrt(8n+1)) */
    uint64_t r = (s - 1) / 2;            /* initial estimate, may need increment */
    while ((r * (r + 1) / 2) < n) r++;

    /* position inside row (0‑based) */
    uint64_t before = (r - 1) * r / 2;
    uint64_t pos = n - before - 1;

    /* --- build array of items for row r --- */
    item_t *items = (item_t *)malloc(r * sizeof(item_t));
    if (!items) return (seq_t)0;  /* allocation failure: return 0 as sentinel */

    for (uint64_t i = 0; i < r; ++i) {
        items[i].val = i + 1;
        items[i].fld = floor_sqrt5(i + 1);
    }

    /* --- sort according to descending fractional parts --- */
    qsort(items, r, sizeof(item_t), cmp_desc);

    /* --- fetch answer and clean up --- */
    uint64_t ans = items[pos].val;
    free(items);
    return (seq_t)ans;
}
