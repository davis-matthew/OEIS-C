/*
 * A069006  Let M denote the 5 X 5 matrix with rows
 *          /1,1,1,1,1/1,1,1,1,0/1,1,1,0,0/1,1,0,0,0/1,0,0,0,0/ and A(n) =
 *          vector (x(n),y(n),z(n),t(n),u(n)) = M^n*A where A is the vector
 *          (1,1,1,1,1); then a(n) = t(n).
 * https://oeis.org/A069006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 4*a(n-3) - a(n-4) + a(n-5) with
 *            a(-5)=0, a(-4)=-1, a(-3)=a(-2)=a(-1)=0, a(0)=1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t max_computed = 4;   /* 0‑based index of last stored value */

    /* one‑time allocation and initialisation of first 5 terms */
    if (cache == NULL) {
        cache_len = 16;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;           /* allocation failure */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 9;
        cache[3] = 29;
        cache[4] = 105;
        for (idx_t i = 5; i < cache_len; ++i)
            cache[i] = 0;
    }

    /* enlarge buffer if required */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        if (new_len < 16) new_len = 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;           /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_len; i < new_len; ++i)
            cache[i] = 0;
        cache_len = new_len;
    }

    /* compute any missing terms up to n */
    if (n > max_computed) {
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            /* signed 128‑bit intermediate guarantees no overflow for
               the 64‑bit build and works for the arbitrary‑precision
               build because the type seq_t overloads the arithmetic
               operators. */
            __int128 temp =
                (__int128)cache[k-1] * 3
              + (__int128)cache[k-2] * 3
              - (__int128)cache[k-3] * 4
              - (__int128)cache[k-4]
              + (__int128)cache[k-5];
            cache[k] = (seq_t)temp;
        }
        max_computed = n;
    }

    return cache[n];
}
