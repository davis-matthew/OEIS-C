/*
 * A185712  a(n) = number of primes <= n that end in 3.
 * https://oeis.org/A185712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count primes ≤ n that end in 3; implemented with a lazy sieve that
   expands to the requested limit using the utility function
   build_primes. */
#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* Static cache of primes up to the greatest limit requested so far. */
    static idx_t *prime_list = NULL;   /* array of primes ≤ prime_limit */
    static size_t prime_count = 0;     /* number of primes in prime_list  */
    static idx_t prime_limit = 0;      /* current maximum bound of the sieve */

    /* Rebuild the prime list if a larger bound is needed. */
    if (n > prime_limit) {
        /* build_primes allocates a new array and sets *out and *cnt. */
        idx_t *new_list = NULL;
        size_t new_cnt = 0;
        build_primes(n, &new_list, &new_cnt);

        /* Free the old list and store the new one. */
        free(prime_list);
        prime_list  = new_list;
        prime_count = new_cnt;
        prime_limit = n;
    }

    /* Count primes ending in digit 3. */
    seq_t count = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > n) break;          /* primes are sorted ascending */
        if ((p % 10) == 3) ++count;
    }
    return count;
}
