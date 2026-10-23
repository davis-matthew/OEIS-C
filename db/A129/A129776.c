/*
 * A129776  Number of maximally-clustered hexagon-avoiding permutations in
 *          S_n; the maximally-clustered hexagon-avoiding permutations are
 *          those that avoid 3421, 4312, 4321, 46718235, 46781235, 56718234,
 *          56781234.
 * https://oeis.org/A129776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of maximally‑clustered hexagon‑avoiding
 *            permutations in S_n
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (from the rational OGF):                     */
/*   a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=78, a(6)=298; */
/*   for n >= 6:  a(n) = 7*a(n-1) - 15*a(n-2) + 14*a(n-3) */
/*                 - 8*a(n-4)  - 4*a(n-5)  + 3*a(n-6);      */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t len = 0;               /* number of values currently in cache */

    /* initial values for indices 0..5 */
    const seq_t init[6] = { (seq_t)1, (seq_t)1, (seq_t)2,
                            (seq_t)6, (seq_t)21, (seq_t)78 };

    /* first call: create cache and store the first six values */
    if (len == 0) {
        size_t initial = 16;
        if (initial < 6) initial = 6;
        cache = (seq_t *)malloc(initial * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failed */
        cap   = initial;
        for (idx_t i = 0; i < 6; ++i) cache[i] = init[i];
        len = 6;
    }

    /* if n is within known cached range, return it directly */
    if (n < len) return cache[n];

    /* extend the cache up to size n+1 */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = cap;
        while (newcap < (size_t)(n + 1)) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = len; i <= n; ++i) {
        /* recurrence valid for i >= 6, but we guard nonetheless */
        seq_t pos = ((seq_t)7  * cache[i-1])
                  + ((seq_t)14 * cache[i-3])
                  + ((seq_t)3  * cache[i-6]);

        seq_t neg = ((seq_t)15 * cache[i-2])
                  + ((seq_t)8  * cache[i-4])
                  + ((seq_t)4  * cache[i-5]);

        cache[i] = pos - neg;   /* pos >= neg for all valid n */
    }
    len = n + 1;
    return cache[n];
}
