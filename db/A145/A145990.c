/*
 * A145990  Primes which start a run of at least length 2 of consecutive
 *          primes == 1 (mod 4).
 * https://oeis.org/A145990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 867, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145990: primes that start a run of at least two consecutive primes ≡ 1 (mod 4). */
/* The implementation increments the prime search, tracks runs of 1‑mod‑4 primes, and stores the
   start of each run when the run ends. The sequence values are memoised in a static array
   so that unordered calls to a(idx_t) return the correct value. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *prime_start_cache = NULL;   /* memoised run starters */
static idx_t  cache_len  = 0;             /* number of stored starters */
static idx_t  cache_cap  = 0;             /* allocated capacity of cache */
static idx_t  next_test  = 2;             /* next integer to test for primeness */
static idx_t  run_len    = 0;             /* length of current 1‑mod‑4 run */
static seq_t  run_start  = 0;             /* first prime of the current run */

/* Expand the cache if necessary. */
static void expand_cache(void)
{
    if (cache_len >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1024;
        seq_t *tmp = (seq_t *)realloc(prime_start_cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort */
            abort();
        }
        prime_start_cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; undefined for n==0 */

    /* Expand cache to contain at least n entries. */
    while (cache_len < n) {
        idx_t x = next_test;
        bool prime = is_prime(x);

        if (prime) {
            if (((x & 3) == 1)) {          /* prime ≡ 1 (mod 4) */
                if (run_len == 0) {
                    run_start = (seq_t)x;
                    run_len = 1;
                } else {
                    run_len++;
                }
            } else {                       /* prime ≡ 3 (mod 4) ends the run */
                if (run_len >= 2) {
                    expand_cache();
                    prime_start_cache[cache_len++] = run_start;
                }
                run_len = 0;
            }
        }

        next_test++;                       /* advance to next candidate */
    }

    return prime_start_cache[n - 1];
}
