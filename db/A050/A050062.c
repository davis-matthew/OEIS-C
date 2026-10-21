/*
 * A050062  a(n) = a(n-1)+a(m), where m=n-1-2^p and 2^p<n-1<=2^(p+1), for n >=
 *          4.
 * https://oeis.org/A050062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 196, u32 n <= 1716, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050062: a(n) = a(n-1) + a(m) where
   m = (n-1) - 2^p and 2^p < (n-1) ≤ 2^(p+1).
   Initial values: a(1)=1, a(2)=3, a(3)=2. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t max_computed = 0;

    if (n == 0)
        return 0;                     /* outside defined range, never used */

    /* Allocate or grow cache array to fit index n */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t*)calloc(cap + 1, sizeof(seq_t)); /* index 0 unused */
    } else if (cap < n) {
        idx_t newcap = cap ? cap * 2 : 4;
        while (newcap < n) newcap <<= 1;
        cache = (seq_t*)realloc(cache, (newcap + 1) * sizeof(seq_t));
        memset(cache + cap + 1, 0, (newcap - cap) * sizeof(seq_t));
        cap = newcap;
    }

    /* Compute values up to n if needed */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else if (i == 2) {
                cache[2] = (seq_t)3;
            } else if (i == 3) {
                cache[3] = (seq_t)2;
            } else {
                idx_t boundary = i - 1;   /* n-1 */
                idx_t mp = 1;             /* largest power of two < boundary */
                while ((mp << 1) < boundary)
                    mp <<= 1;
                idx_t m = boundary - mp;
                cache[i] = cache[i - 1] + cache[m];
            }
        }
        max_computed = n;
    }

    return cache[n];
}
