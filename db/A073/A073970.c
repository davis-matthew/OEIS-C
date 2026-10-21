/*
 * A073970  Number of strings of length n over Z_5 with trace 1 and subtrace
 *          4.
 * https://oeis.org/A073970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recurrence a(1)=0, a(2)=1, a(3)=6, a(4)=30, a(5)=125, a(6)=600,
 *            a(7)=3025 for n≥8: a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) +
 *            50*a(n-4) - 375*a(n-5) + 750*a(n-6) - 625*a(n-7) (derived from
 *            the supplied generating function)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0; return 0 safely */
    if (cache_len < n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure (unlikely in tests) */
        cache = tmp;
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            switch (i) {
            case 1: cache[i] = (seq_t)0; break;
            case 2: cache[i] = (seq_t)1; break;
            case 3: cache[i] = (seq_t)6; break;
            case 4: cache[i] = (seq_t)30; break;
            case 5: cache[i] = (seq_t)125; break;
            case 6: cache[i] = (seq_t)600; break;
            case 7: cache[i] = (seq_t)3025; break;
            default:
                /* positive part of recurrence */
                seq_t pos = (seq_t)10 * cache[i - 1]
                          + (seq_t)50 * cache[i - 3]
                          + (seq_t)50 * cache[i - 4]
                          + (seq_t)750 * cache[i - 6];
                /* negative part of recurrence */
                seq_t neg = (seq_t)35 * cache[i - 2]
                          + (seq_t)375 * cache[i - 5]
                          + (seq_t)625 * cache[i - 7];
                cache[i] = pos - neg;
            }
        }
        cache_len = n;
    }
    return cache[n];
}
