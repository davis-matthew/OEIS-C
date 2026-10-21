/*
 * A230985  Number of white square subarrays of (n+1)X(4+1) binary arrays with
 *          no element equal to a strict majority of its diagonal and
 *          antidiagonal neighbors, with upper left element zero.
 * https://oeis.org/A230985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A230985  –  Empirical linear recurrence (order 18)
    a(n) = 14*a(n-2) – 61*a(n-4) + 148*a(n-6) – 289*a(n-8)
           + 325*a(n‑10) – 227*a(n‑12) + 95*a(n‑14) – 28*a(n‑16)
           + 4*a(n‑18)
    Initial values: a(1)=3, a(2)=5, a(3)=24, a(4)=47, a(5)=191, a(6)=396,
                    a(7)=1573, a(8)=3317, a(9)=13176, a(10)=27805,
                    a(11)=110739, a(12)=233632, a(13)=931361, a(14)=1965325,
                    a(15)=7836336, a(16)=16538039, a(17)=65947255, a(18)=139182070.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* safety, though n≥1 per spec */

    static seq_t *cache = NULL;   /* dynamic cache, 1‑based indexing */
    static idx_t cache_len = 0;   /* allocated size of cache */
    static idx_t computed = 0;    /* highest index already stored */

    /* Ensure the cache can hold indices up to n (and all required initial terms). */
    if (cache_len < n + 1 || cache_len < 19) {
        idx_t need = (n + 1 > 19) ? n + 1 : 19;   /* at least up to 18 */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                       /* allocation failure */
        cache = tmp;
        cache_len = need;
        /* if we just allocated the array, initialise the first 18 terms if not yet done */
        if (computed < 18) {
            static const seq_t init[19] = {0, 3, 5, 24, 47, 191, 396, 1573, 3317,
                                            13176, 27805, 110739, 233632, 931361,
                                            1965325, 7836336, 16538039, 65947255,
                                            139182070};
            for (idx_t i = 1; i <= 18; ++i)
                cache[i] = init[i];
            computed = 18;
        }
    }

    if (n <= computed)
        return cache[n];

    /* Compute missing terms up to n, using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 2] * (seq_t)14;
        val -= cache[i - 4] * (seq_t)61;
        val += cache[i - 6] * (seq_t)148;
        val -= cache[i - 8] * (seq_t)289;
        val += cache[i -10] * (seq_t)325;
        val -= cache[i -12] * (seq_t)227;
        val += cache[i -14] * (seq_t)95;
        val -= cache[i -16] * (seq_t)28;
        val += cache[i -18] * (seq_t)4;
        cache[i] = val;
    }

    computed = n;
    return cache[n];
}
