/*
 * A164022  a(n) = the smallest prime that, when written in binary, starts
 *          with the substring of n in binary.
 * https://oeis.org/A164022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest prime whose binary representation starts
 *            with the binary representation of n
 * Range:     u8 n <= 37, u16 n <= 2084, u32 n <= 6986, u64 n <= 9147,
 *            u128 n <= 9147, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate over increasing numbers of trailing bits until a prime is found. */

#include <stdbool.h>
#include <stddef.h>               /* for size_t */
#include "../utils/is_prime_big.h"/* provides bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1).  The binary prefix of n is the
       first part of the candidate primes.  For each number of
       additional trailing bits k (k = 0,1,2,…) we search the interval
       [n << k, ((n+1) << k) – 1] for the first prime.  Because the
       intervals are strictly increasing, the first prime found is
       the required value. */
    seq_t prefix     = (seq_t)n;        /* binary representation of n */
    seq_t next_prefix = prefix + (seq_t)1;

    for (unsigned k = 0; ; ++k) {
        seq_t low  = prefix << k;
        seq_t high = (next_prefix << k) - (seq_t)1;

        for (seq_t p = low; p <= high; ++p) {
            if (is_prime_big(p))
                return p;
        }
        /* If no prime was found in this interval, continue with
           larger k.  The search is guaranteed to terminate because
           there are infinitely many primes with a given binary prefix
           (see OEIS sequence A018800). */
    }
}
