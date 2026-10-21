/*
 * A212442  G.f.: exp( Sum_{n>=1} A002203(n)^3 * x^n/n ), where A002203 is the
 *          companion Pell numbers.
 * https://oeis.org/A212442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8·a(n‑1) + 76·a(n‑2) + 136·a(n‑3) + 76·a(n‑6) – 38·a(n‑4)
 *            – 136·a(n‑5) – 8·a(n‑7) – a(n‑8) (n ≥ 8), with a(0)=1, a(1)=8,
 *            a(2)=140, a(3)=1864, a(4)=26602, a(5)=373080, a(6)=5253564,
 *            a(7)=73911192
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of terms stored */

    /* grow the array if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms up to n */
    if (len <= (size_t)n) {
        static const seq_t init[8] = {
            (seq_t)1,
            (seq_t)8,
            (seq_t)140,
            (seq_t)1864,
            (seq_t)26602,
            (seq_t)373080,
            (seq_t)5253564,
            (seq_t)73911192
        };

        for (idx_t i = (idx_t)len; i <= n; ++i) {
            seq_t val;
            if (i < 8) {
                val = init[i];
            } else {
                /* compute according to the recurrence */
                val =  (seq_t)8   * cache[i-1];
                val += (seq_t)76  * cache[i-2];
                val += (seq_t)136 * cache[i-3];
                val += (seq_t)76  * cache[i-6];
                val -= (seq_t)38  * cache[i-4];
                val -= (seq_t)136 * cache[i-5];
                val -= (seq_t)8   * cache[i-7];
                val -=           cache[i-8];
            }
            cache[i] = val;
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
