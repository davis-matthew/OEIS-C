/*
 * A180054  In binary expansion, number of 1's in 3n is less than in n.
 * https://oeis.org/A180054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth positive integer m such that popcount(3*m) <
 *            popcount(m), i.e. the A180054 sequence (3‑flimsy numbers)
 * Range:     u8 n <= 14, u16 n <= 5469, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* Count the number of 1‑bits in a seq_t value.  This works for all
   integer types supported by the build, including arbitrary‑precision
   big‑integer classes that provide &, >> and 1. */
static inline int popcount_seq(seq_t x)
{
    int c = 0;
    while (x) {
        c += (int)(x & 1);
        x >>= 1;
    }
    return c;
}

/* Cache for generated values of the sequence.  The cache is grown
   geometrically (double each time) when needed. */
static seq_t *cache_arr = NULL;
static size_t cache_len = 0;   /* number of values currently stored */
static size_t cache_cap = 0;   /* allocated capacity */
static seq_t cur = 1;          /* next candidate integer to test */

/* Return the n-th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for 0, return 0 as sentinel */

    /* Generate sequence values until we have at least n of them. */
    while (cache_len < (size_t)n) {
        int pc_cur  = popcount_seq(cur);
        seq_t three = cur + cur + cur;   /* 3 * cur */
        int pc_three = popcount_seq(three);

        if (pc_three < pc_cur) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc((void *)cache_arr,
                                              new_cap * sizeof(seq_t));
                assert(tmp != NULL);   /* abort on allocation failure */
                cache_arr = tmp;
                cache_cap = new_cap;
            }
            cache_arr[cache_len++] = cur;  /* store the new term */
        }
        cur++;   /* advance to the next integer to test */
    }

    return cache_arr[(size_t)n - 1];
}
