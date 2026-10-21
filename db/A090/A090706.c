/*
 * A090706  Number of numbers having in binary representation the same number
 *          of zeros and ones as n has.
 * https://oeis.org/A090706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binomial( bitlen(n)-1 , popcount(n)-1 )
 * Range:     u8 n <= 2062, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial N choose K with seq_t arithmetic. */
static seq_t binom_uint64(uint64_t N, uint64_t K)
{
    if (K > N) return (seq_t)0;
    if (K == 0 || K == N) return (seq_t)1;
    /* use symmetry to reduce iterations */
    if (K > N - K) K = N - K;

    seq_t res = (seq_t)1;
    for (uint64_t i = 1; i <= K; ++i) {
        /* res = res * (N-K+i) / i  ;  division is exact */
        res = res * (seq_t)(N - K + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Special case: n = 0 has one zero and no ones in its binary form. */
    if (n == 0)
        return (seq_t)1;

    /* Number of bits needed to represent n (most significant 1 included). */
    uint64_t len = 64 - __builtin_clzll((uint64_t)n);   /* len >= 1 for n>0 */

    /* Count of 1-bits in n. */
    uint64_t ones = __builtin_popcountll((uint64_t)n);

    /* We fix the leading 1; remaining bits count is len-1.
       Remaining ones to place (excluding leading 1) is ones-1. */
    uint64_t N = len - 1;       /* positions available in lower bits */
    uint64_t K = ones - 1;      /* ones that must be placed among those positions */

    if (K > N)   /* impossible configuration (e.g., ones > len) */
        return (seq_t)0;

    return binom_uint64(N, K);
}
