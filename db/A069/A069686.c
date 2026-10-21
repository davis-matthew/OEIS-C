/*
 * A069686  Primes whose internal digits form a prime.
 * https://oeis.org/A069686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, is_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose inner (truncated) digits form a prime. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

/* Static cache for memoisation of the sequence */
static seq_t *cache          = NULL;   /* array of found terms   */
static size_t cache_len      = 0;      /* number of terms stored */
static size_t cache_cap      = 0;      /* allocated capacity     */
static idx_t  next_candidate = 2;      /* next integer to test   */

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    while (cache_len < n) {
        /* search for the next prime */
        idx_t p = next_candidate;
        for (;;) {
            ++p;
            if (!is_prime(p)) continue;          /* skip non‑prime */

            /* at least 3 digits required */
            int d = digit_len((seq_t)p);
            if (d >= 3) {
                seq_t P          = (seq_t)p;
                seq_t pow10_val  = pow10((idx_t)(d - 1));
                seq_t inner      = (P % pow10_val) / 10;
                if (is_prime_big(inner)) {
                    /* enlarge cache if needed */
                    if (cache_len == cache_cap) {
                        cache_cap = cache_cap ? cache_cap * 2 : 8;
                        seq_t *tmp = (seq_t *)realloc(cache,
                                                       cache_cap * sizeof(seq_t));
                        if (!tmp) {          /* out of memory */
                            /* keep state consistent for future calls */
                            cache_cap = cache_len = 0;
                            free(cache);
                            cache = NULL;
                            return 0;
                        }
                        cache = tmp;
                    }
                    cache[cache_len++] = P;
                }
            }
            next_candidate = p + 1;
            break;                               /* move to next integer */
        }
    }
    return cache[n - 1];
}
