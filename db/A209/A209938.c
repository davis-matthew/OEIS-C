/*
 * A209938  Number of groups of order prime(n)^5 with nontrivial unramified
 *          Brauer groups.
 * https://oeis.org/A209938
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(prime(n)-1,4) + gcd(prime(n)-1,3) + 1
 * Range:     u8 n <= 54, u16 n <= 6862, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sieve_first_n_primes.h"   /* for sieve_first_n_primes */
#include <stdlib.h>                          /* for free */

seq_t a(idx_t n)
{
    if (n < 3)            /* offset is 3, values below are irrelevant */
        return 0;

    static uint32_t *prime_list = NULL;
    static size_t prime_cnt = 0;

    /* Ensure we have at least n primes in prime_list */
    if (prime_cnt < n) {
        if (prime_list)
            free(prime_list);            /* free old buffer if any */
        sieve_first_n_primes(n, &prime_list, &prime_cnt);
    }

    /* prime(n) is stored at prime_list[n-1] (0‑based index) */
    seq_t p_minus_1 = (seq_t)prime_list[n - 1] - 1;
    seq_t g4 = gcd(p_minus_1, (seq_t)4);
    seq_t g3 = gcd(p_minus_1, (seq_t)3);

    return g4 + g3 + (seq_t)1;
}
