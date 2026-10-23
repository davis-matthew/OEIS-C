/*
 * A165456  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1)+26*a(n-2)+...+26*a(n-8)-351*a(n-9) (n>=10)
 *            a(0)=1, a(1)=28, a(2)=756, a(3)=20412, a(4)=551124,
 *            a(5)=14880348, a(6)=401769396, a(7)=10847773692,
 *            a(8)=292889889684 a(9) = 26*a(8)+...+26*a(1)-351*a(0)+1 (special
 *            case to match the rational G.f.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
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
    if (n < 9) {
        static const seq_t init[9] = {
            (seq_t)1,
            (seq_t)28,
            (seq_t)756,
            (seq_t)20412,
            (seq_t)551124,
            (seq_t)14880348,
            (seq_t)401769396,
            (seq_t)10847773692,
            (seq_t)292889889684
        };
        return init[(size_t)n];
    }

    /* Initialise cache with the first 9 values if needed */
    if (cache_len < 9) {
        idx_t new_len = 9;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        static const seq_t init[9] = {
            (seq_t)1,
            (seq_t)28,
            (seq_t)756,
            (seq_t)20412,
            (seq_t)551124,
            (seq_t)14880348,
            (seq_t)401769396,
            (seq_t)10847773692,
            (seq_t)292889889684
        };
        for (size_t i = 0; i < 9; ++i) cache[i] = init[i];
        cache_len = 9;
    }

    /* Extend cache up to index n */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            sum += (seq_t)26 * cache[i-1];
            sum += (seq_t)26 * cache[i-2];
            sum += (seq_t)26 * cache[i-3];
            sum += (seq_t)26 * cache[i-4];
            sum += (seq_t)26 * cache[i-5];
            sum += (seq_t)26 * cache[i-6];
            sum += (seq_t)26 * cache[i-7];
            sum += (seq_t)26 * cache[i-8];
            sum -= (seq_t)351 * cache[i-9];
            if (i == 9) sum += (seq_t)1;  /* handle the n=9 special term */
            cache[i] = sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}
