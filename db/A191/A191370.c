/*
 * A191370  a(n) = 2*(1+(-1)^n)/3 + 2*A010892(n-1).
 * https://oeis.org/A191370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=4; for n ≥ 0 : a(n+3) = 3*2ⁿ – a(n)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *a_cache    = NULL;
static seq_t *pow2_cache = NULL;
static idx_t cache_len   = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    idx_t old_len = cache_len;
    idx_t new_len = n + 1;

    a_cache    = seq_realloc(a_cache,    (size_t)old_len, (size_t)new_len);
    pow2_cache = seq_realloc(pow2_cache, (size_t)old_len, (size_t)new_len);

    for (idx_t i = old_len; i < new_len; ++i)
    {
        switch (i)
        {
            case 0:
                a_cache[i]    = (seq_t)1;
                pow2_cache[i] = (seq_t)1;
                break;
            case 1:
                a_cache[i]    = (seq_t)2;
                pow2_cache[i] = (seq_t)2;
                break;
            case 2:
                a_cache[i]    = (seq_t)4;
                pow2_cache[i] = (seq_t)4;
                break;
            default:
                /* pow2(i) = 2 * pow2(i-1) */
                pow2_cache[i] = pow2_cache[i - 1] * (seq_t)2;

                /* a(i) = 3 * pow2(i-3) - a(i-3) */
                a_cache[i] = (seq_t)3 * pow2_cache[i - 3] - a_cache[i - 3];
                break;
        }
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
