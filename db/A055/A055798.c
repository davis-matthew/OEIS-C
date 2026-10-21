/*
 * A055798  T(2n+5,n), array T as in A055794.
 * https://oeis.org/A055798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 57, u64 n <= 961,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A055798, T(2n+5,n)
   Formula: a(n) = binomial(n+8,8) - 2*binomial(n+6,7)
   for all n ≥ 0 (binomial(k,m)=0 when m>k). */
#include <stdint.h>

static inline seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;   /* use symmetry */
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply then divide – use 128‑bit to avoid overflow
           for the sizes involved (k ≤ 8, n ≤ 10 000). */
        r = (seq_t)(((__int128)r * (n - k + i)) / i);
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t b1 = binom_small(n + 8, 8);
    seq_t b2 = binom_small(n + 6, 7);
    return b1 - (b2 << 1);
}
