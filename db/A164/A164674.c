/*
 * A164674  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A164674  
   Formula: a(n) = 36*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)) - 666*a(n-7) 
            + [n==7]   (the bracket means 1 if n == 7, 0 otherwise)
   Initial values: a(0)=1, a(1)=38, a(2)=1406, a(3)=52022, a(4)=1924814,
                   a(5)=71218118, a(6)=2635070366
   The recurrence follows from the rational generating function
   (t^7 + 2*t^6 + … + 2*t + 1)/(666*t^7 - 36*t^6 - … - 36*t + 1).
*/

#include <stddef.h>
#include <stdlib.h>

/* The function must have external linkage */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries in cache */

    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort */
            exit(1);
        }
        cache = tmp;
        /* compute all missing entries up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)38;
            } else if (i == 2) {
                cache[2] = (seq_t)1406;
            } else if (i == 3) {
                cache[3] = (seq_t)52022;
            } else if (i == 4) {
                cache[4] = (seq_t)1924814;
            } else if (i == 5) {
                cache[5] = (seq_t)71218118;
            } else if (i == 6) {
                cache[6] = (seq_t)2635070366;
            } else {
                /* recurrence for i >= 7 */
                seq_t sum6 = cache[i-1];
                sum6 += cache[i-2];
                sum6 += cache[i-3];
                sum6 += cache[i-4];
                sum6 += cache[i-5];
                sum6 += cache[i-6];
                seq_t val = (seq_t)36 * sum6;
                val -= (seq_t)666 * cache[i-7];
                if (i == 7) {
                    val += (seq_t)1;   /* coefficient from the numerator */
                }
                cache[i] = val;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
