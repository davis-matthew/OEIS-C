/*
 * A163221  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*a(n-1)+36*a(n-2)+36*a(n-3)-666*a(n-4) with initial
 *            values a(0)=1, a(1)=38, a(2)=1406, a(3)=52022, and for n=4 the
 *            recurrence has a constant term:
 *            a(4)=36*a(3)+36*a(2)+36*a(1)-666*a(0)+1 = 1924111
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- memoisation cache --------------------------------------- */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    static idx_t cache_size = 0;

    /* --- initialise cache on first call -------------------------- */
    if (cache == NULL) {
        cache_capacity = (n + 1) < 5 ? 5 : (idx_t)(n + 1);
        cache = (seq_t*)malloc((size_t)cache_capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;

        /* initialise all slots lazily below */
        cache_size = 0;
    }

    /* --- grow cache if needed ------------------------------------ */
    if (n + 1 > cache_capacity) {
        idx_t new_cap = n + 1;
        if (new_cap < 5) new_cap = 5;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* --- compute missing terms ----------------------------------- */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        switch (i) {
            case 0: val = (seq_t)1; break;
            case 1: val = (seq_t)38; break;
            case 2: val = (seq_t)1406; break;
            case 3: val = (seq_t)52022; break;
            case 4:
                /* a(4) = 36*a(3)+36*a(2)+36*a(1)-666*a(0)+1 */
                val = 36 * cache[3] + 36 * cache[2] + 36 * cache[1]
                      - 666 * cache[0] + (seq_t)1;
                break;
            default:
                /* a(n) = 36*a(n-1)+36*a(n-2)+36*a(n-3)-666*a(n-4)  (n>=5) */
                val = 36 * cache[i-1] + 36 * cache[i-2] + 36 * cache[i-3]
                      - 666 * cache[i-4];
                break;
        }
        cache[i] = val;
    }
    cache_size = n + 1;

    return cache[n];
}
