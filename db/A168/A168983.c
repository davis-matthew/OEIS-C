/*
 * A168983  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1)+16*a(n-2)+⋯+16*a(n-22) - 136*a(n-23) + b(n)
 *            where b(n) = 1 for n = 0 or 23, 2 for 1 ≤ n ≤ 22, 0 otherwise.
 *            This follows from the rational generating function
 *            (t^23+2t^22+…+2t+1)/(1-16t-…-16t^22+136t^23)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   b_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/b_val.h"

static seq_t *cache = NULL;     /* memoised sequence values            */
static size_t cached_cap = 0;   /* current allocation capacity         */
static idx_t   cached_len = 0;  /* number of terms stored (≤ cached_cap) */

seq_t a(idx_t n)
{
    /* Allocate (or enlarge) cache to hold up to n. */
    if ((size_t)(n + 1) > cached_cap) {
        size_t new_cap = (cached_cap == 0) ? 1 : cached_cap * 2;
        while (new_cap < (size_t)(n + 1)) new_cap *= 2;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure – caller should not call in that case */
        if (cache) {
            for (size_t i = 0; i < cached_len; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cached_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t sum = 0;
        /* add 16 * a(i-k) for k=1..22 where applicable */
        if (i >= 1) sum += (seq_t)16 * cache[i - 1];
        if (i >= 2) sum += (seq_t)16 * cache[i - 2];
        if (i >= 3) sum += (seq_t)16 * cache[i - 3];
        if (i >= 4) sum += (seq_t)16 * cache[i - 4];
        if (i >= 5) sum += (seq_t)16 * cache[i - 5];
        if (i >= 6) sum += (seq_t)16 * cache[i - 6];
        if (i >= 7) sum += (seq_t)16 * cache[i - 7];
        if (i >= 8) sum += (seq_t)16 * cache[i - 8];
        if (i >= 9) sum += (seq_t)16 * cache[i - 9];
        if (i >= 10) sum += (seq_t)16 * cache[i - 10];
        if (i >= 11) sum += (seq_t)16 * cache[i - 11];
        if (i >= 12) sum += (seq_t)16 * cache[i - 12];
        if (i >= 13) sum += (seq_t)16 * cache[i - 13];
        if (i >= 14) sum += (seq_t)16 * cache[i - 14];
        if (i >= 15) sum += (seq_t)16 * cache[i - 15];
        if (i >= 16) sum += (seq_t)16 * cache[i - 16];
        if (i >= 17) sum += (seq_t)16 * cache[i - 17];
        if (i >= 18) sum += (seq_t)16 * cache[i - 18];
        if (i >= 19) sum += (seq_t)16 * cache[i - 19];
        if (i >= 20) sum += (seq_t)16 * cache[i - 20];
        if (i >= 21) sum += (seq_t)16 * cache[i - 21];
        if (i >= 22) sum += (seq_t)16 * cache[i - 22];
        /* subtract 136 * a(i-23) if applicable */
        if (i >= 23) sum -= (seq_t)136 * cache[i - 23];
        /* add inhomogeneous part b(i) */
        sum += b_val(i);
        cache[i] = sum;
    }

    cached_len = n + 1;
    return cache[n];
}
