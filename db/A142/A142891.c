/*
 * A142891  Primes congruent to 4 mod 63.
 * https://oeis.org/A142891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   the n‑th prime of the form 63*k + 4 (OEIS A142891)
 * Range:     u8 n <= 2, u16 n <= 189, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached primes of the form 63k+4 */
    static size_t  cap     = 0;      /* current capacity of cache      */
    static idx_t   count   = 0;      /* number of primes stored         */
    static idx_t   k       = 0;      /* current multiplier for 63k+4    */

    /* initialise cache */
    if (cache == NULL) {
        cap   = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed */
    }

    /* extend cache until it contains at least n primes */
    while (count < n) {
        /* compute candidate 63*k + 4 using 64‑bit arithmetic
           to avoid overflow for large k before casting to seq_t   */
        uint64_t cand64 = (uint64_t)63 * (uint64_t)k + 4ULL;
        seq_t  cand = (seq_t)cand64;     /* cast – safe for the useful ranges */

        if (isPrimeGeneric(cand)) {
            if (count >= cap) {                 /* grow array when needed */
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (tmp == NULL) {               /* allocation failed */
                    free(cache);
                    cache = NULL;
                    count = 0;
                    return 0;
                }
                cache = tmp;
            }
            cache[(size_t)count++] = cand;      /* store found prime     */
        }
        ++k;
    }

    return cache[(size_t)(n - 1)];   /* a(1) is stored at index 0 */
}
