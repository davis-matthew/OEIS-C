/*
 * A052538  Expansion of (1-x)/(1-2*x-3*x^2+3*x^3).
 * https://oeis.org/A052538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 3*a(n-3), with a(0)=1, a(1)=1,
 *            a(2)=5
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache   = NULL;
    static idx_t size     = 0;   /* number of terms currently stored */
    static idx_t capacity = 0;   /* allocated capacity */

    /* Ensure we have space for terms up to n */
    if (n >= size) {
        idx_t need = n + 1;
        if (need > capacity) {
            idx_t newcap = capacity ? (capacity << 1) : 16;
            if (newcap < need) newcap = need;
            seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = newptr;
            capacity = newcap;
        }
        for (idx_t i = size; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)1;
            else if (i == 2)
                cache[i] = (seq_t)5;
            else {
                seq_t t1 = cache[i-1] * (seq_t)2;
                seq_t t2 = cache[i-2] * (seq_t)3;
                seq_t t3 = cache[i-3] * (seq_t)3;
                cache[i] = t1 + t2 - t3;
            }
        }
        size = need;
    }

    return cache[n];
}
