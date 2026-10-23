/*
 * A129974  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+937)^2 = y^2.
 * https://oeis.org/A129974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 1874 for n > 6 a(1)=0, a(2)=627,
 *            a(3)=1128, a(4)=2811, a(5)=6188, a(6)=9027
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = (seq_t*)0;   /* cache[i] = a(i+1), 0‑based */
static size_t cache_cap = 0;       /* current allocated size */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t need = (size_t)n;
    size_t old_cap = cache_cap;
    if (need > old_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp && need) abort();
        cache = tmp;
        /* initialise new slots to 0 (base values will overwrite) */
        for (size_t i = old_cap; i < need; ++i) cache[i] = (seq_t)0;
    }

    /* compute missing entries */
    for (size_t i = old_cap; i < need; ++i) {
        if (i < 6) { /* base cases a(1)…a(6) */
            switch ((idx_t)i) {
                case 0: cache[i] = (seq_t)0;     break;
                case 1: cache[i] = (seq_t)627;   break;
                case 2: cache[i] = (seq_t)1128;  break;
                case 3: cache[i] = (seq_t)2811;  break;
                case 4: cache[i] = (seq_t)6188;  break;
                case 5: cache[i] = (seq_t)9027;  break;
            }
        } else { /* recurrence for n > 6 */
            seq_t val = (seq_t)6 * cache[i - 3];
            val = val - cache[i - 6];
            val = val + (seq_t)1874;
            cache[i] = val;
        }
    }
    cache_cap = need;
    return cache[n - 1]; /* 1‑based index → 0‑based element */
}
