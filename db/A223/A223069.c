/*
 * A223069  Number of n X 2 0..3 arrays with successive rows and columns
 *          fitting to straight lines with nondecreasing slope, with a single
 *          point array taken as having zero slope.
 * https://oeis.org/A223069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) -106*a(n-2) +376*a(n-3) -769*a(n-4) +
 *            904*a(n-5) -564*a(n-6) +144*a(n-7) Initial values: a(1)=16,
 *            a(2)=150, a(3)=1080, a(4)=6627, a(5)=36552, a(6)=187000,
 *            a(7)=905440
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Compute the nth term of OEIS sequence A223069 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* undefined for non‑positive n */

    /* extend cache if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;             /* allocation failed */

        /* copy already computed values */
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];

        /* compute missing values from (old_cap) to (new_cap-1) */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            idx_t idx = (idx_t)(i + 1);  /* 1‑based index */
            seq_t val;
            switch (idx) {
                case 1: val = (seq_t)16; break;
                case 2: val = (seq_t)150; break;
                case 3: val = (seq_t)1080; break;
                case 4: val = (seq_t)6627; break;
                case 5: val = (seq_t)36552; break;
                case 6: val = (seq_t)187000; break;
                case 7: val = (seq_t)905440; break;
                default: {
                    /* compute using recurrence with only additions and a final subtraction */
                    seq_t pos = (seq_t)16 * new_cache[i-1]
                               + (seq_t)376 * new_cache[i-3]
                               + (seq_t)904 * new_cache[i-5]
                               + (seq_t)144 * new_cache[i-7];
                    seq_t neg = (seq_t)106 * new_cache[i-2]
                               + (seq_t)769 * new_cache[i-4]
                               + (seq_t)564 * new_cache[i-6];
                    val = pos - neg;  /* pos >= neg for all n >= 8 */
                }
            }
            new_cache[i] = val;
        }

        /* replace old cache */
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    return cache[(size_t)n - 1];
}
