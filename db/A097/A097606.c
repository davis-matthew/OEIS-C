/*
 * A097606  a(n) = number of terms among {a(1), a(2), a(3), ..., a(n-1)} that
 *          are coprime to n; a(1)=6.
 * https://oeis.org/A097606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms among {a(1),...,a(n-1)} that are coprime
 *            to n; a(1) = 6
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *a_cache = NULL;       /* cache of a(1)..a(n)  (index 0 unused)   */
static idx_t  a_cache_len = 0;      /* highest n for which a_cache[n] is valid */
static idx_t  a_cache_cap = 0;      /* allocated capacity (elements)         */

/* Return a(n).  Works for all n >= 1.  Subsequent calls are cached.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset below 1, value irrelevant */

    /* Resize cache if needed */
    if (n > a_cache_cap) {
        idx_t new_cap = a_cache_cap ? a_cache_cap << 1 : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp) { /* allocation failed – abort */ abort(); }
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* Initialise first term if it has not been set yet */
    if (a_cache_len < 1) {
        a_cache[1] = (seq_t)6;
        a_cache_len = 1;
    }

    /* Compute missing values up to n */
    for (idx_t i = a_cache_len + 1; i <= n; ++i) {
        seq_t count = 0;
        seq_t snum = (seq_t)i;
        for (idx_t j = 1; j < i; ++j)
            if (gcd(a_cache[j], snum) == (seq_t)1)
                ++count;
        a_cache[i] = count;
    }
    if (n > a_cache_len) a_cache_len = n;

    return a_cache[n];
}
