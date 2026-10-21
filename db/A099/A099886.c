/*
 * A099886  XOR binomial transform of A099885.
 * https://oeis.org/A099886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = SumXOR_{k=0..n} SumXOR_{i=0..floor(k/2)} (C(n,k) mod
 *            2)·(C(floor(k/2), i) mod 2)·2^{((k+1)/2)+floor(k/2)-i}
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified: a(n) = XOR_{k subset of n} XOR_{i subset of floor(k/2)} 1 << (k - i) */

/* All computation is done using the integer types seq_t and idx_t supplied by the build system.
   No other includes or external functions are required. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t ans = 0;
    const size_t maxbits = sizeof(seq_t) * 8;   /* maximum exponent that can be represented */

    /* Iterate over all subsets k of the set of bits of n */
    for (idx_t k = n; ; k = (k == 0) ? (idx_t)-1 : (k - 1) & n) {
        idx_t m = k >> 1;  /* floor(k/2) */
        /* Iterate over all subsets i of the set of bits of m */
        for (idx_t i = m; ; i = (i == 0) ? (idx_t)-1 : (i - 1) & m) {
            idx_t e = k - i;   /* exponent for this (k,i) pair */
            if (e < maxbits)   /* ignore bits beyond the width of seq_t */
                ans ^= ((seq_t)1 << (unsigned)e);
            if (i == 0) break;  /* finished all submasks of m */
        }
        if (k == 0) break;      /* finished all submasks of n */
    }
    return ans;
}
