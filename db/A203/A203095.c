/*
 * A203095  Number of n X 2 0..3 arrays with every nonzero element less than
 *          or equal to some horizontal or vertical neighbor.
 * https://oeis.org/A203095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 10*a(n-1) - a(n-2) + 236*a(n-3) + 270*a(n-4) +
   945*a(n-5) + 532*a(n-6) + 576*a(n-7);
   initial values: a(1)=4, a(2)=84, a(3)=1080, a(4)=12260, a(5)=143510,
   a(6)=1705124, a(7)=20174774. */
#include <stdlib.h>

static const seq_t init_vals[7] = {(seq_t)4,      (seq_t)84,     (seq_t)1080,
                                   (seq_t)12260, (seq_t)143510, (seq_t)1705124,
                                   (seq_t)20174774};

static seq_t *cache = NULL;          /* cache[0] unused, cache[1] = a(1), ...     */
static size_t cache_cap = 0;         /* allocated size of cache                 */
static idx_t  computed = 0;          /* highest index computed and stored       */

static void ensure_cache(idx_t n)
{
    /* ensure cache is allocated up to index n */
    if (n >= cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 8;
        while (newcap <= (size_t)n) newcap *= 2;
        seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = newptr;
        cache_cap = newcap;
    }
    /* initialize the first seven values once */
    if (!computed) {
        for (int i = 0; i < 7; ++i) {
            cache[i+1] = init_vals[i];
        }
        /* cache[0] is unused but set to zero for safety */
        cache[0] = 0;
        computed = 7;
    }
}

seq_t a(idx_t n)
{
    /* return the n-th term of the sequence (n >= 1) */
    if (n <= 7) return init_vals[(int)n - 1];

    ensure_cache(n);

    for (idx_t i = computed + 1; i <= n; ++i) {
        /* recurrence computation */
        seq_t val = (seq_t)10 * cache[i-1]
                    - cache[i-2]
                    + (seq_t)236 * cache[i-3]
                    + (seq_t)270 * cache[i-4]
                    + (seq_t)945 * cache[i-5]
                    + (seq_t)532 * cache[i-6]
                    + (seq_t)576 * cache[i-7];
        cache[i] = val;
    }
    computed = n;
    return cache[n];
}
