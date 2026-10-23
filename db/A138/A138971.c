/*
 * A138971  Positions of the primes congruent to 3 mod 4 when all primes
 *          except 2 are listed in order.
 * https://oeis.org/A138971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all odd numbers, testing for primality,
   counting the primes, and recording the positions of those primes
   congruent to 3 (mod 4).  a(n) is the position of the n‑th such
   prime in the list of all primes except 2. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, guard for safety */

    /* Static state retained across calls */
    static seq_t *cache = NULL;     /* array of positions for 3 mod 4 primes */
    static size_t cap   = 0;        /* allocated capacity of cache         */
    static size_t size  = 0;        /* number of entries currently stored   */
    static idx_t  candidate   = 1;  /* next odd number to test (starts at 3)*/
    static idx_t  overall_cnt = 0;  /* count of primes seen (excluding 2)   */

    /* Ensure the cache has at least n entries. */
    if (cache == NULL || size < n) {
        /* Allocate or grow the cache. */
        if (cache == NULL) {
            cap = 128;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            if (!cache) return 0; /* allocation failure; return 0 as fallback */
        } else if (size >= cap) {
            size_t newcap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return 0; /* realloc failure; return 0 as fallback */
            cache = tmp;
            cap = newcap;
        }

        /* Generate new primes until we have n entries. */
        while (size < n) {
            /* Advance to the next odd candidate. */
            candidate += 2;                                 /* skip even */
            if (is_prime(candidate)) {                      /* prime?  */
                ++overall_cnt;                            /* position */
                if ((candidate & ((idx_t)3)) == 3) {       /* 3 mod 4?  */
                    /* store position */
                    cache[size++] = (seq_t)overall_cnt;
                }
            }
        }
    }

    return cache[n - 1];  /* 1‑based index in the sequence */
}
