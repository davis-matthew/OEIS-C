/*
 * A067763  Square array read by antidiagonals of base n numbers written as
 *          122...222 with k 2's (and a suitable interpretation for n=0, 1 or
 *          2).
 * https://oeis.org/A067763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   a(n) = T(i, j) where (i, j) are found by reading the * infinite
 *            array T(n, k) (n ≥ 0, k ≥ 0) by antidiagonals. * T(n, 0) = 1, *
 *            T(n, k) = n * T(n, k-1) + 2 (k ≥ 1). * This gives the OEIS
 *            sequence A067763
 * Range:     u8 n <= 38, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t compute_T(idx_t n, idx_t k)
{
    seq_t t = 1;           /* T(n,0) */
    for (idx_t i = 0; i < k; ++i) {
        t = (seq_t)n * t + 2;
    }
    return t;
}

seq_t a(idx_t idx)
{
    /* Find s such that idx < C(s) where C(s) = (s+1)(s+2)/2. */
    __uint128_t cum = 1;          /* cumulative count up to current diagonal s=0 */
    idx_t s = 0;

    while ( (__uint128_t)idx >= cum ) {
        ++s;
        cum += s + 1;             /* Next diagonal adds (s+1) entries */
    }

    /* Cum_before = saturated count up to s-1 = s*(s+1)/2. */
    __uint128_t cum_before = (__uint128_t)s * (s + 1) / 2;
    idx_t offset = idx - (idx_t)cum_before;
    idx_t n = offset;            /* row index */
    idx_t k = s - offset;        /* column index */

    return compute_T(n, k);
}
