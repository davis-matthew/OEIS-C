/*
 * A151755  a(n) = 0 for n <= 4; for n >= 5, write n = 2^i + j, 0 <= j < 2^i;
 *          if j <= 2^i-2 then a(n) = 2*a(j) + a(j+1); if j = 2^i-1 then a(n)
 *          = 2*a(j) + a(j+1) + 1.
 * https://oeis.org/A151755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 0, 0 ≤ n ≤ 4; For n ≥ 5: let n = 2^i + j with 0 ≤ j <
 *            2^i. If j ≤ 2^i-2 then a(n) = 2·a(j) + a(j+1); If j = 2^i-1 then
 *            a(n) = 2·a(j) + a(j+1) + 1
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   floor_log2u64, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/floor_log2u64.h"

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache        = NULL;
    static idx_t cache_size    = 0;   /* size of allocated array      */
    static idx_t computed      = 0;   /* number of entries computed   */

    /* expand cache if necessary */
    if (n >= computed)
    {
        idx_t new_size = n + 1;
        if (new_size < 20) new_size = 20;                /* initial space            */

        /* allocate new array, copy old values, keep pointer */
        seq_t *new_cache = seq_alloc((size_t)new_size);
        for (idx_t i = 0; i < computed; ++i)
            new_cache[i] = cache[i];
        cache = new_cache;
        cache_size = new_size;

        /* compute new values */
        for (idx_t k = computed; k <= n; ++k)
        {
            if (k <= 4)
            {
                cache[k] = (seq_t)0;
            }
            else
            {
                idx_t i   = floor_log2u64(k);         /* 2^i ≤ k < 2^(i+1)      */
                idx_t pow2 = 1ull << i;              /* 2^i                    */
                idx_t j   = k - pow2;                /* 0 ≤ j < 2^i            */

                seq_t val = cache[j];
                val += val;              /* 2 * a(j)                 */
                val += cache[j+1];       /* + a(j+1)                 */
                if (j == pow2 - 1)       /* last element of block    */
                    val += (seq_t)1;     /* +1 for the special case  */

                cache[k] = val;
            }
        }
        computed = n + 1;
    }

    return cache[n];
}
