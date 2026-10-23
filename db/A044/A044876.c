/*
 * A044876  Numbers having, in base 5, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having, in base 5, sum of even run lengths = sum of odd run lengths. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether a number satisfies the property. */
static inline bool satisfies(seq_t val)
{
    seq_t tmp = val;
    int prev = -1;
    unsigned run_len = 0;
    unsigned sum_odd = 0;
    unsigned sum_even = 0;

    if (tmp == 0) return false;                 /* 0 has one odd run of length 1 */

    while (tmp > 0) {
        int digit = (int)(tmp % (seq_t)5);
        if (digit == prev) {
            run_len++;
        } else {
            if (prev != -1) {
                if (run_len % 2 == 0) sum_even += run_len;
                else                 sum_odd  += run_len;
            }
            prev = digit;
            run_len = 1;
        }
        tmp /= (seq_t)5;
    }
    /* process last run */
    if (run_len % 2 == 0) sum_even += run_len;
    else                  sum_odd  += run_len;

    return sum_odd == sum_even;
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_sz  = 0;
    static idx_t   last_val  = 0;   /* last integer examined */

    if (n == 0) return (seq_t)0;    /* undefined for n < 1 */

    /* expand cache if needed */
    while (cache_sz < (size_t)n) {
        idx_t x = last_val + 1;                 /* next integer to test */
        while (!satisfies((seq_t)x)) ++x;

        /* grow capacity if necessary */
        if (cache_sz == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            seq_t *new_arr;

#ifdef __cplusplus
            new_arr = new seq_t[new_cap];
            for (size_t i = 0; i < cache_sz; ++i) new_arr[i] = cache[i];
            delete[] cache;
#else
            new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_arr) abort();
#endif
            cache = new_arr;
            cache_cap = new_cap;
        }

        cache[cache_sz++] = (seq_t)x;
        last_val = x;
    }
    return cache[n - 1];
}
