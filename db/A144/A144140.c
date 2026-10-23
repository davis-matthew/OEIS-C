/*
 * A144140  Numbers n such that between n^K and (n+1)^K there are no primes,
 *          where K = 3/2.
 * https://oeis.org/A144140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A144140: the n‑th integer n such
   that no prime lies strictly between n^(3/2) and (n+1)^(3/2). */
#include <stdint.h>
#include <stdbool.h>

/* Utility functions provided by the build system. */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

static inline bool is_square_uint64(idx_t x)
{
    seq_t sx = (seq_t)x;
    seq_t r  = floor_sqrt(sx);
    return r * r == sx;
}

static inline bool qualifies(idx_t n)
{
    seq_t nseq = (seq_t)n;

    /* n^(3/2) = sqrt(n^3) */
    seq_t n3   = nseq * nseq * nseq;
    seq_t low  = floor_sqrt(n3) + (seq_t)1;   /* first integer > n^(3/2) */

    /* (n+1)^(3/2) */
    idx_t n1   = n + (idx_t)1;
    seq_t n1seq = (seq_t)n1;
    seq_t n13  = n1seq * n1seq * n1seq;
    seq_t high = floor_sqrt(n13);             /* floor((n+1)^(3/2)) */

    /* If (n+1)^(3/2) is integer, exclude that endpoint. */
    if (is_square_uint64(n1)) {
        if (high > (seq_t)0) high -= (seq_t)1;
    }

    if (low > high) return true;              /* empty interval */

    /* Scan the interval for a prime. */
    for (seq_t p = low; p <= high; ++p) {
        if (is_prime_big(p)) return false;    /* a prime was found */
    }
    return true;                              /* no primes in the interval */
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;       /* undefined for offset 0 */

    idx_t count = 0;
    idx_t candidate = 1;

    while (count < n) {
        if (qualifies(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }

    /* Should never reach here. */
    return (seq_t)candidate;
}
