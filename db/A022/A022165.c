/*
 * A022165  First row of spectral array W(sqrt(5)-1).
 * https://oeis.org/A022165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 6*a(n-2) - 4*a(n-4), a(0)=1, a(1)=5, a(2)=6, a(3)=26
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_len = 0;
static size_t seq_cache_cap = 0;

seq_t a(idx_t n)
{
    static const seq_t base[4] = { (seq_t)1, (seq_t)5, (seq_t)6, (seq_t)26 };

    /* Ensure cache capacity */
    if ((size_t)(n + 1) > seq_cache_cap)
    {
        size_t new_cap = (size_t)(n + 1);
        if (seq_cache_cap == 0)
        {
            seq_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        }
        else
        {
            seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        }
        seq_cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = seq_cache_len; i <= n; ++i)
    {
        if (i < 4)
        {
            seq_cache[i] = base[i];
        }
        else
        {
            seq_cache[i] = (seq_t)6 * seq_cache[i - 2] - (seq_t)4 * seq_cache[i - 4];
        }
    }
    seq_cache_len = n + 1;

    return seq_cache[n];
}
