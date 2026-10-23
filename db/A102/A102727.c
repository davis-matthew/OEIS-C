/*
 * A102727  Numbers k such that k1111111 is prime.
 * https://oeis.org/A102727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102727 – Numbers k such that k·1111111 is prime.
   The numbers are found by searching k = 6,7,8,… and testing
   the primality of candidate = k*10^7 + 1111111.  The function
   returns the n‑th such k (a(1) = 6). */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"   /* is_prime_uint64(uint64_t) */

/* The arithmetic progression: 10 000 000 per step, with offset 1 111 111. */
static const uint64_t STEP   = 10000000ULL;   /* 10^7 */
static const uint64_t OFFSET = 1111111ULL;    /* 1 111 111 */

seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many primes found so far */
    for (idx_t k = 6; ; ++k)  /* the sequence starts with k = 6 */
    {
        /* Form the candidate number k*10^7 + 1 111 111.  The
           multiplication fits in 64 bits for all k used in this
           problem (k up to a few hundred thousand). */
        uint64_t cand = (uint64_t)k * STEP + OFFSET;

        /* Prime test using the utility function.  It is
           deterministic for 64‑bit numbers. */
        if (is_prime_uint64(cand))
        {
            ++count;          /* one more member of the sequence */
            if (count == n)   /* the *n*‑th one is found */
                return (seq_t)k;
        }
    }
}
