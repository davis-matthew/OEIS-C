/*
 * A063712  Table of bits required for product of n- and k-bit positive
 *          numbers read by antidiagonals.
 * https://oeis.org/A063712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = if n == 1 then 1 for any n >= 1, we map the index n to
 *            the pair (i, j) of the infinite matrix of a(i, j), read by
 *            antidiagonals, and then return a(i, j) where a(i, j) = i*j if
 *            min(i, j) == 1 else i + j
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_uint64(idx_t x) {
    /* Integer square root of a 64‑bit unsigned integer. */
    idx_t low = 0, high = (idx_t)1 << 32; /* 2^32 > sqrt(2^63-1)  */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * (unsigned long long)mid;
        if (sq == x) return mid;
        if (sq < x)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;      /* floor(sqrt(x)) */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of range */
    /* Find diagonal sum t (i+j) such that the first t−1 diagonals contain
       less than n entries.  Number of elements with sum ≤ t is (t−1)t/2. */
    idx_t m = 1 + 8 * n;              /* 1+8n fits in 64‑bit for all practical n */
    idx_t s = isqrt_uint64(m);        /* floor sqrt */
    idx_t t = (1 + s) >> 1;           /* floor((1+sqrt(1+8n))/2) */
    if ( (unsigned __int128)(t - 1) * t / 2 < n ) ++t; /* ceil correction */

    /* Number of elements before diagonal t */
    idx_t before = (t - 2) * (t - 1) / 2;
    idx_t offset = n - before - 1;          /* 0‑based index inside the diagonal */
    idx_t i = 1 + offset;                   /* row index */
    idx_t j = t - i;                        /* column index */

    /* Compute the value according to the definition */
    if (i == 1 || j == 1) {
        return (seq_t)i * (seq_t)j;
    } else {
        return (seq_t)i + (seq_t)j;
    }
}
