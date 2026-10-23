/*
 * A029388  Expansion of 1/((1-x^5)*(1-x^6)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029388: Number of partitions of n into parts 5, 6, 9, and 11.
   Recurrence: a(n) = a(n‑5)+a(n‑6)+a(n‑9)
                  –a(n‑14)–a(n‑15)–a(n‑16)–a(n‑17)
                  +a(n‑22)+a(n‑25)+a(n‑26)–a(n‑31)
   (Terms with negative indices are treated as 0; a(0)=1, a(n)=0 for 1≤n<5.) */
#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic storage of a(0)…a(n)          */
static idx_t cache_size = 0;         /* number of entries currently stored    */

static const int k_values[11] = {5, 6, 9, 14, 15, 16, 17, 22, 25, 26, 31};
static const int k_signs[11]  = { 1, 1, 1, -1, -1, -1, -1,  1,  1,   1, -1 };

/* compute a(n) for any n≥0, storing intermediate results for reuse */
seq_t a(idx_t n) {
    /* grow the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;                 /* we need indices 0…n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();              /* allocation failure */
        cache = tmp;

        /* old_size is the number of elements that were already computed */
        idx_t old_size = cache_size;
        cache_size = new_size;

        /* initialise a(0) if this is the first allocation */
        if (old_size == 0) {
            cache[0] = (seq_t)1;
            old_size = 1;                      /* we have now filled 0 */
        }

        /* compute new entries */
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t sum = 0;
            for (int j = 0; j < 11; ++j) {
                int k = k_values[j];
                if (i >= (idx_t)k) {
                    seq_t term = cache[i - k];
                    if (k_signs[j] > 0) sum += term;
                    else                sum -= term;
                }
            }
            cache[i] = sum;
        }
    }
    return cache[n];
}
