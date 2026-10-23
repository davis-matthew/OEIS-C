/*
 * A022031  Define the sequence T(a(0),a(1)) by a(n+2) is the greatest integer
 *          such that a(n+2)/a(n+1) < a(n+1)/a(n) for n >= 0. This is T(4,17).
 * https://oeis.org/A022031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=4, a(1)=17, a(n+1) = ceil(a(n)^2/a(n-1)) – 1 for n>0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_sz = 0;
static idx_t seq_cache_cap = 0;

seq_t a(idx_t n)
{
    /* Allocate or grow cache as necessary */
    if (!seq_cache)
    {
        idx_t capacity = (n > 1 ? n + 1 : 2);
        seq_cache_cap = capacity;
        seq_cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        seq_cache[0] = (seq_t)4;
        seq_cache[1] = (seq_t)17;
        seq_cache_sz = 2; /* first two terms are ready */
    }
    else if (n + 1 > seq_cache_sz)
    {
        if (n + 1 > seq_cache_cap)
        {
            seq_cache_cap = (seq_cache_cap * 2 > n + 1) ? seq_cache_cap * 2 : n + 1;
            seq_cache = (seq_t *)realloc(seq_cache, seq_cache_cap * sizeof(seq_t));
        }
        /* compute missing terms using recurrence */
        for (idx_t i = seq_cache_sz; i <= n; ++i)
        {
#ifdef __cplusplus
            /* Using native arithmetic of seq_t (suitable for arbitrary‑precision) */
            seq_cache[i] = ((seq_cache[i-1] * seq_cache[i-1] + seq_cache[i-2] - (seq_t)1) / seq_cache[i-2]) - (seq_t)1;
#else
            /* Use 128‑bit intermediate for fixed‑width integer types */
            unsigned __int128 a = (unsigned __int128)seq_cache[i-1];
            unsigned __int128 b = (unsigned __int128)seq_cache[i-2];
            unsigned __int128 val = (a * a + b - (unsigned __int128)1) / b - (unsigned __int128)1;
            seq_cache[i] = (seq_t)val;
#endif
        }
        seq_cache_sz = n + 1;
    }
    return seq_cache[n];
}
