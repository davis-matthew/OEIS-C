/*
 * A049967  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 3, and a(3) = 1.
 * https://oeis.org/A049967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=1. For n >= 4: a(n) = a(1)+a(2)+...+a(n-1)
 *            + a(m), where m = 2*n - 2 - 2^(p+1) and p is the unique integer
 *            with 2^p < n-1 <= 2^(p+1)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL; /* cache[i] holds a(i+1) */
    static size_t cap      = 0;   /* allocated size of cache */
    static idx_t max_computed = 0;/* largest index computed */
    static seq_t running_sum = 0; /* sum of a(1) .. a(max_computed) */

    /* Ensure the cache can hold up to n elements. */
    if (n > max_computed) {
        size_t needed = (size_t)n;
        if (cap < needed) {
            size_t newcap = (cap == 0 ? 4 : cap << 1);
            while (newcap < needed) newcap <<= 1;
            cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cap = newcap;
        }

        /* Compute new terms from max_computed+1 up to n. */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
            } else if (i == 2) {
                val = (seq_t)3;
            } else if (i == 3) {
                val = (seq_t)1;
            } else {
                /* Find p such that 2^p < i-1 <= 2^(p+1). */
                idx_t k = i - 1;
                idx_t power = 1; /* 2^p < k   holds initially */
                while ((power << 1) < k)
                    power <<= 1;
                idx_t pow2next = power << 1;               /* 2^(p+1) */
                idx_t m = 2 * i - 2 - pow2next;            /* 1 <= m <= i-1 */
                val = running_sum + cache[m - 1];
            }
            cache[i - 1] = val;
            running_sum += val;
        }
        max_computed = n;
    }
    return cache[n - 1];
}
