/*
 * A230081  Sequence needed for the nonpositive powers of rho(11) =
 *          2*cos(Pi/11) in the power basis of the degree 5 number field
 *          Q(rho(11)). Negative of the coefficients of the second power.
 * https://oeis.org/A230081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 4*a(n-3) - a(n-4) + a(n-5), a(0) =
 *            0, a(1) = 4, a(2) = 13, a(3) = 50, a(4) = 173
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_len)
        return;

    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!cache)
        abort();

    for (idx_t i = cache_len; i < new_len; ++i)
    {
        switch (i)
        {
            case 0:
                cache[0] = (seq_t)0;
                break;
            case 1:
                cache[1] = (seq_t)4;
                break;
            case 2:
                cache[2] = (seq_t)13;
                break;
            case 3:
                cache[3] = (seq_t)50;
                break;
            case 4:
                cache[4] = (seq_t)173;
                break;
            default:
            {
                seq_t t1 = 3 * cache[i - 1] + 3 * cache[i - 2] + cache[i - 5];
                seq_t t2 = 4 * cache[i - 3] + cache[i - 4];
                cache[i] = t1 - t2;
            }
        }
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
