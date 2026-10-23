/*
 * A011846  a(n) = floor( binomial(n,9)/10 ).
 * https://oeis.org/A011846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( binomial(n,9)/10 )
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 67, u64 n <= 744,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 9, binomial(n,9) = 0, so a(n) = 0. */
    if (n < 9) return 0;

#if defined(__SIZEOF_INT128__)  /* use 128‑bit arithmetic when available */
    __uint128_t prod = 1;
    for (idx_t i = 0; i < 9; ++i) {
        prod *= (uint64_t)(n - i);
    }
    /* 9! * 10 = 3628800 */
    prod /= 3628800ULL;
    return (seq_t)prod;
#else   /* fallback to arbitrary‑precision binomial */
    seq_t binom = binomial(n, 9);
    return binom / 10;
#endif
}
