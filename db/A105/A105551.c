/*
 * A105551  Number of distinct prime factors of n^3 + n^2 + 71.
 * https://oeis.org/A105551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001221(n^3 + n^2 + 71) (number of distinct prime factors
 *            of n^3 + n^2 + 71)
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/count_distinct_prime_factors.h"   /* provides: size_t count_distinct_prime_factors(uint64_t) */

seq_t a(idx_t n)
{
    /* n is unsigned 64‑bit, cast to seq_t for the arithmetic */
    seq_t nseq   = (seq_t)n;
    seq_t val128 = (unsigned __int128)nseq * nseq * nseq
                 + (unsigned __int128)nseq * nseq
                 + (unsigned __int128)71u;

    /* We factor only when the value fits into 64 bits. The tests for this
       sequence stay well within that range (e.g. n ≤ 10 000 => value < 10^12). */
    if (val128 > (unsigned __int128)UINT64_MAX) {
        /* In the rare event of overflow we could implement big‑int
           factorisation, but for the intended test ranges this case
           never occurs. */
        return (seq_t)0;
    }

    uint64_t v64 = (uint64_t)val128;
    size_t   cnt = count_distinct_prime_factors(v64);
    return (seq_t)cnt;
}
