/*
 * A072222  a(n) = (abs(n-1-a(n-2)) mod n) + (abs(n-1-a(n-1)) mod (n-1)), a(0)
 *          = 1, a(1) = 1.
 * https://oeis.org/A072222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (abs(n-1-a(n-2)) mod n) + (abs(n-1-a(n-1)) mod (n-1))
 * with a(0)=1, a(1)=1.  For n>6 it simplifies to a(n)=a(n-3)+2.
 * This implementation precomputes values up to the largest n seen
 * and stores them in a dynamic array.  The recurrence is used for
 * n≥7; for 0≤n≤6 the explicit values are hard‑coded.
 */
#include <stdlib.h>

static seq_t *cache = NULL;        /* dynamic array of computed terms   */
static idx_t  cache_len = 0;       /* number of terms stored (size)     */

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;          /* already computed */

    idx_t new_len = n + 1;              /* need indices 0..n          */
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) __builtin_trap();   /* allocation failure          */
    cache = new_cache;

    /* Fill new entries.  Existing entries (0..cache_len-1) are preserved. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 6) {                   /* hard‑coded base values      */
            switch (i) {
                case 0: cache[i] = 1; break;
                case 1: cache[i] = 1; break;
                case 2: cache[i] = 0; break;
                case 3: cache[i] = 1; break;
                case 4: cache[i] = 5; break;
                case 5: cache[i] = 4; break;
                case 6: cache[i] = 1; break;
            }
        } else {                        /* recurrence for n > 6        */
            cache[i] = cache[i - 3] + (seq_t)2;
        }
    }
    cache_len = new_len;
}

/* Public entry point for the OEIS sequence. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
