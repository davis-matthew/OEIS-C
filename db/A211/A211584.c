/*
 * A211584  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having
 *          four, five, six or seven distinct values for every i,j,k<=n.
 * https://oeis.org/A211584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)+10*a(n-2)-35*a(n-3)-25*a(n-4)+130*a(n-5) *
 *            -180*a(n-7)+36*a(n-8)+72*a(n-9) (empirical, for n ≥ 10) *
 *            Initial values for 1 ≤ n ≤ 9: * a(1)=16, a(2)=48, a(3)=128,
 *            a(4)=302, a(5)=704, * a(6)=1622, a(7)=3696, a(8)=8626,
 *            a(9)=19784 * * The function memoizes the sequence so that
 *            arbitra...
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: 1‑based indexing, cache[0] unused */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size in elements */
    static idx_t  cache_last = 0;  /* highest n for which cache[n] is valid */

    if (n == 0) return 0; /* out of range, not used by the OEIS */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16; /* start with room for indices up to 15 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure → return 0 as failure code */

        /* base values for indices 1 … 9 */
        cache[0] = 0; /* unused slot, keeps the array 1‑based */
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)48;
        cache[3] = (seq_t)128;
        cache[4] = (seq_t)302;
        cache[5] = (seq_t)704;
        cache[6] = (seq_t)1622;
        cache[7] = (seq_t)3696;
        cache[8] = (seq_t)8626;
        cache[9] = (seq_t)19784;
        cache_last = 9;
    }

    /* enlarge the cache array if requested index is outside current capacity */
    while ((size_t)n >= cache_cap) {              /* need index n, so size at least n+1 */
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 9) {
            /* base values already stored, skip computation */
            continue;
        }
        /* compute a(i) using the recurrence while avoiding unsigned underflow */
        /* positive part: 3*a(i-1)+10*a(i-2)+130*a(i-5)+36*a(i-8)+72*a(i-9) */
        seq_t pos = 0;
        pos += 3 * cache[i - 1];
        pos += 10 * cache[i - 2];
        pos += 130 * cache[i - 5];
        pos += 36 * cache[i - 8];
        pos += 72 * cache[i - 9];

        /* negative part: 35*a(i-3)+25*a(i-4)+180*a(i-7) */
        seq_t neg = 0;
        neg += 35 * cache[i - 3];
        neg += 25 * cache[i - 4];
        neg += 180 * cache[i - 7];

        cache[i] = pos - neg; /* pos ≥ neg for all i ≥ 10 */
    }

    cache_last = (cache_last > (idx_t)n) ? cache_last : (idx_t)n;
    return cache[n];
}
