/*
 * A131183  a(n) = a(n-1) + a(n-2) if n == 3 mod 4; a(n) = a(n-1) - a(n-2) if
 *          n == 0 mod 4; a(n) = a(n-1)*a(n-2) if n == 1 mod 4; and a(n) =
 *          a(n-1)/a(n-2) if n == 2 mod 4; with a(1)=a(2)=1.
 * https://oeis.org/A131183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2) if n%4==3; a(n)=a(n-1)-a(n-2) if n%4==0;
 *            a(n)=a(n-1)*a(n-2) if n%4==1; a(n)=a(n-1)/a(n-2) if n%4==2;
 *            a(1)=a(2)=1
 * Range:     u8 n <= 16, u16 n <= 20, u32 n <= 24, u64 n <= 28, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache   = NULL;   /* cache[i] holds a(i+1) */
static size_t cache_cap = 0;    /* allocated size */
static size_t cache_len = 0;    /* number of terms stored */

seq_t a(idx_t n)
{
    if (n < 1)                     /* undefined for n < 1, harmless */
        return (seq_t)0;

    if (n <= 2)                    /* a(1)=a(2)=1 */
        return (seq_t)1;

    /* initialise cache if needed */
    if (cache_len < 2) {
        cache_cap = 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;        /* a(1) */
        cache[1] = (seq_t)1;        /* a(2) */
        cache_len = 2;
    }

    /* grow cache to at least n terms */
    if (cache_len < (size_t)n) {
        size_t new_cap = (size_t)n;
        if (cache_cap < new_cap) {
            cache_cap = new_cap;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t a_prev      = cache[cache_len - 1];
            seq_t a_prevprev  = cache[cache_len - 2];
            seq_t next;
            switch (i & 3ULL) {          /* i % 4 */
                case 3: next = a_prev + a_prevprev; break;   /* addition */
                case 0: next = a_prev - a_prevprev; break;   /* subtraction */
                case 1: next = a_prev * a_prevprev; break;   /* multiplication */
                default:                     /* case 2 */
                    next = a_prevprev == 0 ? (seq_t)0 : a_prev / a_prevprev;
                    break;                     /* division */
            }
            cache[cache_len] = next;
            ++cache_len;
        }
    }

    return cache[n - 1];
}
