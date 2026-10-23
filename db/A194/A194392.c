/*
 * A194392  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) < 0, where
 *          r=sqrt(13) and < > denotes fractional part.
 * https://oeis.org/A194392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let r = sqrt(13). Count lowCount = #k ≤ m with fractional part *
 *            of k*r < 1/2. Include m iff 2*lowCount < m. * This is equivalent
 *            to the definition: Sum_{k=1..m}(<1/2 + k*r> - <k*r>) < 0
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

static seq_t *cache = NULL;  /* cached sequence terms */
static idx_t cache_cap = 0;  /* capacity of cache */
static idx_t cache_len = 0;  /* number of terms stored */

/* next m to examine and cumulative lowCount */
static idx_t next_m = 1;
static idx_t lowCount = 0;

seq_t a(idx_t n) {
    /* Return the n‑th term (1‑based) of A194392. */
    if (n == 0) return 0;   /* guard against illegal call */

    /* Generate terms until we have at least n terms in the cache. */
    while (cache_len < n) {
        seq_t m = (seq_t)next_m;              /* current m as seq_t */
        seq_t m2 = m * m;                     /* m^2 */

        /* a = floor(m*sqrt(13)) = floor(sqrt(13*m^2)) */
        seq_t a_val = floor_sqrt((seq_t)13 * m2);

        /* Check condition f_k < 1/2: 52*m^2 < 4*a^2 + 4*a + 1 */
        seq_t left  = (seq_t)52 * m2;
        seq_t a2    = a_val * a_val;
        seq_t right = (seq_t)4 * a2 + (seq_t)4 * a_val + (seq_t)1;

        if (left < right) {
            /* fractional part of m*sqrt(13) is less than 1/2 */
            lowCount++;
        }

        /* Include m if 2*lowCount < m. */
        if ((lowCount << 1) < next_m) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – abort with zero, as error handling
                     * is beyond this function's responsibility. */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }

        next_m++;
    }

    return cache[n - 1];
}
