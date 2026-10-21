/*
 * A073124  a(n) = prime(1+prime(n)) - prime(prime(n)).
 * https://oeis.org/A073124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(1+prime(n)) - prime(prime(n))
 * Range:     u8 n <= 54, u16 n <= 7093, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation pre‑computes a single sieve up to LIMIT = 2 000 000, 
   stores all primes in a static array, and answers every query in O(1). 
   The routine works for any seq_t that supports the usual arithmetic
   operations and conversion to unsigned long long.  */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static data for lazy initialisation -------------------------------- */
    static seq_t *prime_arr = NULL;
    static size_t prime_cnt = 0;
    static bool initialized = false;

    const idx_t LIMIT = 2000000;                /* enough for n≈10 000       */

    if (!initialized) {
        /* 1. Build a simple sieve of Eratosthenes. ----------------------- */
        uint8_t *sieve = (uint8_t*)malloc((size_t)LIMIT + 1);
        if (!sieve) return 0;                  /* allocation failure */

        memset(sieve, 0, (size_t)LIMIT + 1);

        /* 2. Rough upper bound on π(LIMIT) (≈ LIMIT / ln LIMIT). */
        const size_t EST = LIMIT / 10 + 10;
        prime_arr = (seq_t*)malloc(EST * sizeof(seq_t));
        if (!prime_arr) { free(sieve); return 0; }

        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (!sieve[i]) {
                prime_arr[prime_cnt++] = (seq_t)i;   /* store prime */
                if (i <= LIMIT / i) {
                    for (idx_t j = i * i; j <= LIMIT; j += i)
                        sieve[j] = 1;                 /* mark composites */
                }
            }
        }
        free(sieve);
        initialized = true;
    }

    /* ----- answer the query -------------------------------------------- */
    if (n < 1 || (size_t)n > prime_cnt) return 0;   /* out of bounds */

    seq_t p = prime_arr[n - 1];                     /* p = prime(n)       */
    unsigned long long ui = (unsigned long long)p;  /* safe cast: p <= LIMIT */

    if (ui == 0 || ui + 1 > prime_cnt) return 0;    /* safety check */

    seq_t lower = prime_arr[ui - 1];                /* prime(prime(n))   */
    seq_t upper = prime_arr[ui];                    /* prime(prime(n)+1)*/
    return upper - lower;                          /* desired value     */
}
