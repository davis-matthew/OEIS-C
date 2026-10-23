/*
 * A187592  Number of 9-step E, S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n>10 (initial values:
 *            a(1)=0, a(2)=0, a(3)=13, a(4)=2792, a(5)=27252, a(6)=103108,
 *            a(7)=246792, a(8)=458018, a(9)=732810, a(10)=1069534)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const idx_t _base_vals[11] = {
    0ULL,         /* unused 0th index */
    0ULL,         /* a(1) */
    0ULL,         /* a(2) */
    13ULL,        /* a(3) */
    2792ULL,      /* a(4) */
    27252ULL,     /* a(5) */
    103108ULL,    /* a(6) */
    246792ULL,    /* a(7) */
    458018ULL,    /* a(8) */
    732810ULL,    /* a(9) */
    1069534ULL    /* a(10) */
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety; not used in OEIS */

    static seq_t *cache = NULL;           /* array 0..computed_upto */
    static idx_t computed_upto = 0;       /* largest n for which cache is filled */

    /* Expand cache if necessary */
    if (n > computed_upto) {
        size_t new_size = (size_t)(n + 1); /* include index 0 */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }

        /* Fill values */
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = (seq_t)_base_vals[i];
            } else {
                /* recurrence: a(i) = 3*a(i-1) - 3*a(i-2) + a(i-3) */
                cache[i] = 3 * cache[i - 1] - 3 * cache[i - 2] + cache[i - 3];
            }
        }
        computed_upto = n;
    }

    return cache[n];
}
