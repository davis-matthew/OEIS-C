/*
 * A024675  Average of two consecutive odd primes.
 * https://oeis.org/A024675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (prime(n+1)+prime(n+2))/2
 * Range:     u8 n <= 52, u16 n <= 6540, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_limit_for_idx, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the average of two consecutive odd primes.      */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"   /* sieve_first_n_primes()   */
#include "../utils/prime_limit_for_idx.h"   /* _prime_limit_for_idx()   */

static uint32_t *prime_list = NULL;   /* Array of the smallest primes   */
static size_t   prime_count = 0;      /* Number of primes in prime_list */

/* Return the n-th term of sequence A024675.  n >= 1.            */
seq_t a(idx_t n)
{
    /* Ensure we have at least (n+2) primes stored.  The first prime
     * is prime_list[0] = 2.  The n-th term requires prime(n+1)
     * and prime(n+2) -> prime_list[n] and prime_list[n+1].       */
    if (prime_list == NULL || (size_t)(n + 2) > prime_count) {
        idx_t needed = n + 2;                    /* number of primes required   */
        size_t cnt = 0;
        uint32_t *pr = NULL;

        sieve_first_n_primes(needed, &pr, &cnt); /* build first `needed` primes */

        /* Replace old list with the new one.  Free old memory if present. */
        free(prime_list);
        prime_list = pr;
        prime_count = cnt;
    }

    /* Compute average of prime(n+1) and prime(n+2). */
    seq_t p1 = (seq_t)prime_list[(size_t)n];
    seq_t p2 = (seq_t)prime_list[(size_t)n + 1];
    return (p1 + p2) / 2;
}
