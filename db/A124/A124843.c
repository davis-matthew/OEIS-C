/*
 * A124843  Number of base 8 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 8*a(n-1)
 *            -15*a(n-2) -10*a(n-3) +29*a(n-4) +4*a(n-5) -12*a(n-6) * initial
 *            values: a(0)=1, a(1)=8, a(2)=34, a(3)=122, a(4)=502, a(5)=2098,
 *            a(6)=8980
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_len = 0;
static idx_t seq_computed_up_to = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough to hold indices up to n */
    if ((idx_t)(n + 1) > seq_cache_len) {
        size_t newlen = (size_t)(n + 1);
        if (newlen < 7u)
            newlen = 7u;
        if (seq_cache == NULL) {
            seq_cache = (seq_t *)malloc(newlen * sizeof(seq_t));
        } else {
            seq_cache = (seq_t *)realloc(seq_cache, newlen * sizeof(seq_t));
        }
        seq_cache_len = (idx_t)newlen;
        /* Initialize the first seven terms, which are needed for the recurrence */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)8;
        seq_cache[2] = (seq_t)34;
        seq_cache[3] = (seq_t)122;
        seq_cache[4] = (seq_t)502;
        seq_cache[5] = (seq_t)2098;
        seq_cache[6] = (seq_t)8980;
    }

    /* Make sure we have computed at least up to index 6 */
    if (seq_computed_up_to < 6)
        seq_computed_up_to = 6;

    /* Compute any terms that have not yet been computed */
    if (n > seq_computed_up_to) {
        for (idx_t i = seq_computed_up_to + 1; i <= n; ++i) {
            seq_t a_n1 = seq_cache[i - 1];
            seq_t a_n2 = seq_cache[i - 2];
            seq_t a_n3 = seq_cache[i - 3];
            seq_t a_n4 = seq_cache[i - 4];
            seq_t a_n5 = seq_cache[i - 5];
            seq_t a_n6 = seq_cache[i - 6];

            seq_t total = (seq_t)8 * a_n1 + (seq_t)29 * a_n4 + (seq_t)4 * a_n5;
            seq_t sub   = (seq_t)15 * a_n2 + (seq_t)10 * a_n3 + (seq_t)12 * a_n6;
            seq_cache[i] = total - sub;
        }
        seq_computed_up_to = n;
    }

    return seq_cache[n];
}
