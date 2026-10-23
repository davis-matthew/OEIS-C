/*
 * A165329  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A165329
 *
 *  Recurrence derived from the rational generating function:
 *
 *      A(t) = (t^9 + 2t^8 + 2t^7 + 2t^6 + 2t^5 + 2t^4 + 2t^3 + 2t^2 + 2t + 1)
 *            / (136t^9 - 16t^8 - 16t^7 - 16t^6 - 16t^5 - 16t^4 - 16t^3 - 16t^2 - 16t + 1)
 *
 *  Let Q(t)=1-16t-16t^2-…-16t^8+136t^9.  Then for n≥9
 *
 *      a(n) = 16·(a(n‑1)+a(n‑2)+…+a(n‑8)) - 136·a(n‑9) + p(n)
 *
 *  with p(9)=1 and p(n)=0 for n≠9.  The first nine values are fixed:
 *      a(0)=1, a(1)=18, a(2)=306, a(3)=5202, a(4)=88434,
 *      a(5)=1503378, a(6)=25557426, a(7)=434476242, a(8)=7386096114.
 *
 *  The function below memoises the sequence in a dynamic array so that
 *  calls with arbitrary indices in any order are O(n) overall.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;          /* number of terms currently stored */

    /* the known first nine terms */
    static const seq_t init[9] = {
        (seq_t)1ULL,
        (seq_t)18ULL,
        (seq_t)306ULL,
        (seq_t)5202ULL,
        (seq_t)88434ULL,
        (seq_t)1503378ULL,
        (seq_t)25557426ULL,
        (seq_t)434476242ULL,
        (seq_t)7386096114ULL
    };

    /* if the requested value is within the initial block, return it directly */
    if (n < 9)
        return init[(size_t)n];

    /* initialise cache (on first call) */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        if (cache_cap < 16)
            cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        cache_len = 9;
    }

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute terms up to index n if they are not yet known */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; k <= 8; ++k)
            sum += cache[i - k];

        seq_t val = (seq_t)16 * sum;
        val -= (seq_t)136 * cache[i - 9];
        if (i == 9)          /* add the numerator term for n = 9 */
            val += (seq_t)1;
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
