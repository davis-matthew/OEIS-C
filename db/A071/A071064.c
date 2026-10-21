/*
 * A071064  Numbers divisible by the sum of factorials of their digits
 *          [A061602(n)] and also terminate in the sum of factorials of their
 *          digits.
 * https://oeis.org/A071064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers that are divisible by the sum of factorials of their
   digits AND terminate in that sum.  This is OEIS A071064. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Pre‑computed factorials 0! … 9! */
static const seq_t fact[10] = {
    (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)6,
    (seq_t)24, (seq_t)120, (seq_t)720, (seq_t)5040,
    (seq_t)40320, (seq_t)362880
};

/* Cache of found terms */
static seq_t *cache          = NULL;
static size_t cache_len      = 0;
static size_t cache_cap      = 0;
static seq_t  current_search = (seq_t)1;   /* next integer to test */

/* Compute sum of factorials of the decimal digits of x */
static seq_t sum_fact_digits(seq_t x)
{
    if (x == (seq_t)0) return (seq_t)1;      /* 0! = 1 */
    seq_t s = (seq_t)0;
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        s += fact[d];
        x /= (seq_t)10;
    }
    return s;
}

/* Grow the cache array */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16ull;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);
    cache = new_arr;
    cache_cap = new_cap;
}

/* Append a new term to the cache */
static void add_to_cache(seq_t val)
{
    if (cache_len == cache_cap) grow_cache();
    cache[cache_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* offset is 1; undefined for 0 */

    /* Generate terms up to required index */
    while (cache_len < n) {
        seq_t cur = current_search;
        seq_t sum = sum_fact_digits(cur);

        if (sum <= cur) {
            if (cur % sum == (seq_t)0) {
                int len_sum = digit_len(sum);
                seq_t pow10k = pow10((idx_t)len_sum);
                if (cur % pow10k == sum) {
                    add_to_cache(cur);
                }
            }
        }
        ++current_search;
    }
    return cache[n - 1];
}
