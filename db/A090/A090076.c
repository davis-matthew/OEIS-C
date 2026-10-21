/*
 * A090076  a(n) = prime(n)*prime(n+2).
 * https://oeis.org/A090076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) * prime(n+2) where prime(1)=2, prime(2)=3,
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Ensure we have at least n+2 primes computed */
    static uint64_t *prime_list = NULL;   /* array of primes */
    static idx_t  prime_count    = 0;      /* number of primes stored */
    static idx_t  max_needed     = 0;      /* last index of required primes */
    static unsigned char *is_prime = NULL; /* sieve buffer */
    static idx_t  sieve_limit    = 0;      /* upper bound used for sieve */

    if (n < 1) return 0; /* undefined for n < 1 */

    idx_t needed = n + 2;
    if (needed > max_needed) {
        /* Need to recompute primes up to at least 'needed' */
        if (prime_list) free(prime_list);
        if (is_prime)  free(is_prime);

        /* Estimate an upper bound for the needed prime using Rosser's theorem */
        idx_t bound;
        if (needed < 6) {
            bound = 15;          /* first few primes lie below 15 */
        } else {
            double m = (double)needed;
            /* p_n < n (ln n + ln ln n) for n >= 6 */
            double est = m * (log(m) + log(log(m))) + 3.0;
            bound = (idx_t)est;
        }

        /* Allocate sieve array */
        is_prime = (unsigned char*)malloc((size_t)(bound + 1));
        if (!is_prime) return 0; /* allocation fail – unlikely in test */

        for (size_t i = 0; i <= bound; ++i)
            is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        for (idx_t i = 2; i * i <= bound; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= bound; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Extract primes up to 'needed' count */
        prime_list = (uint64_t*)malloc((size_t)needed * sizeof(uint64_t));
        if (!prime_list) return 0; /* allocation fail – unlikely */

        idx_t cnt = 0;
        for (idx_t i = 2; i <= bound && cnt < needed; ++i) {
            if (is_prime[i]) {
                prime_list[cnt++] = (uint64_t)i;
            }
        }
        /* Stash metadata for next call */
        prime_count = cnt;
        max_needed  = needed;
        sieve_limit = bound;
    }

    /* Now we safely have at least n+2 primes */
    /* prime(n)  -> prime_list[n-1]
       prime(n+2)-> prime_list[n+1]   (since n+2 is two after n) */
    seq_t p1 = (seq_t)prime_list[n - 1];
    seq_t p2 = (seq_t)prime_list[n + 1];
    return p1 * p2;
}
