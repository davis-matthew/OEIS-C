/*
 * A132398  Numbers n such that 11*n^2 + 1 is prime.
 * https://oeis.org/A132398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Computes the n-th integer m such that 11*m^2 + 1 is prime. All
 *            terms are multiples of 6. This implementation lazily generates
 *            terms on demand and caches them for future calls
 * Range:     u8 none, u16 n <= 5, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq.h"

/* Return the n-th element (1‑based) of sequence A132398. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for 0, but avoid UB */

    /* Static state shared across calls */
    static seq_t *cache = NULL;          /* array of found terms   */
    static size_t capacity = 0;          /* allocated length       */
    static size_t count = 0;             /* number of terms cached */
    static idx_t last_k = 0;             /* last processed multiplier (k for m=6*k) */

    /* Extend cache until we have at least n terms */
    while (count < n) {
        ++last_k;                                     /* next multiplier   */
        seq_t m = (seq_t)(6 * last_k);                /* candidate number  */
        seq_t val = (seq_t)11 * m * m + 1;            /* 11*m^2+1 */

        if (isPrimeGeneric(val)) {                    /* primality test    */
            /* Grow the cache if needed */
            if (count == capacity) {
                size_t new_cap = capacity ? capacity * 2 : 8;
                grow_seq(&cache, &capacity, new_cap);
            }
            cache[count++] = m;                        /* store term        */
        }
    }

    return cache[n - 1];
}
