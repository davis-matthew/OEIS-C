/*
 * A142394  Primes congruent to 43 mod 47.
 * https://oeis.org/A142394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 149, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A142394: primes ≡ 43 (mod 47)  (computed by successive testing) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset is 1; n ≥ 1 is the intended usage */

    static seq_t *cache   = NULL; /* stored primes */
    static size_t cap     = 0;    /* storage capacity in elements */
    static size_t len     = 0;    /* number of primes currently cached */
    static idx_t  cand    = 43;   /* next candidate congruent to 43 mod 47 */
    static bool    init    = false;

    if (!init) {                 /* one‑time initialisation */
        cand = 43;
        init = true;
    }

    /* grow the cache until we have at least n primes */
    while (len < (size_t)n) {
        /* test the current candidate */
        if (is_prime((idx_t)cand)) {
            if (len >= cap) {    /* enlarge the cache when full */
                size_t new_cap = cap ? cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)cand;
        }
        cand += 47;               /* next number ≡ 43 (mod 47) */
    }

    return cache[n - 1];          /* 0‑based array, n ≥ 1 */
}
