/*
 * A075290  Interprimes which are of the form s*prime, s=15.
 * https://oeis.org/A075290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   build_primes, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Interprimes of the form 15*prime (s = 15):
   a(n) is the n-th integer that is the average of two consecutive primes
   and equals 15 times a prime.
   Computed by sieving primes and scanning for qualifying averages. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/is_prime_small.h"

/* Sequence entry function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n = 0, but never called */

    static seq_t *cache = NULL;              /* cache of found terms          */
    static size_t cache_len = 0;             /* number of cached terms        */
    static size_t cache_cap = 0;             /* allocated cache capacity      */

    static idx_t *prime_list = NULL;         /* list of primes up to limit    */
    static size_t prime_cnt = 0;             /* number of primes              */
    static idx_t prime_limit = 0;            /* current sieve limit           */

    static size_t last_scan = 1;             /* index of first prime in next pair */

    /* Ensure we have at least n terms in the cache */
    while (cache_len < n) {
        /* Build initial sieve if necessary */
        if (prime_limit == 0) {
            prime_limit = 3000000;        /* initial limit – enough for up to ~10 000 terms */
            build_primes(prime_limit, &prime_list, &prime_cnt);
        }

        /* Scan successive pairs of consecutive primes */
        while (last_scan + 1 < prime_cnt && cache_len < n) {
            idx_t q = prime_list[last_scan];
            idx_t r = prime_list[last_scan + 1];

            /* q and r are odd for last_scan >= 1, so the average is integer */
            seq_t avg = (q + r) / 2;

            if (avg % 15 == 0) {
                seq_t prod = avg / 15;
                if (is_prime_small((idx_t)prod)) { /* prod must be prime */
                    if (cache_len >= cache_cap) {
                        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp) return 0;              /* allocation failure */
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = avg;
                }
            }
            ++last_scan;
        }

        /* If we still need more terms, extend the prime sieve */
        if (cache_len < n) {
            idx_t new_limit = prime_limit + prime_limit;   /* double the range */
            free(prime_list);
            build_primes(new_limit, &prime_list, &prime_cnt);
            prime_limit = new_limit;
            /* last_scan points to the last processed prime,
               so the next pair will start at last_scan and last_scan+1 */
        }
    }

    return cache[n - 1];
}
