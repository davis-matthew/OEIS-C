/*
 * A032720  Integers that when prefixed by '3' and followed by '7' yield a
 *          prime.
 * https://oeis.org/A032720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032720: a(n) = the nth integer k (n≥1) such that the decimal
   number formed by prefixing k with '3' and suffixing it with '7'
   (i.e., 3·10^(digits(k)+1)+k·10+7) is prime.  The function
   computes this value by a linear search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static inline int num_digits(idx_t x)
{
    if (x == 0) return 1;
    int d = 0;
    while (x > 0) {
        x /= 10;
        ++d;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache  = NULL;
    static size_t cap    = 0;
    static size_t found  = 0;
    static idx_t  cur_k  = 0; /* next candidate integer */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
    }

    size_t target = (size_t)n;
    while (found < target) {
        /* number of decimal digits of cur_k */
        int d = num_digits(cur_k);

        /* compute 10^(d+1) as seq_t */
        seq_t pow10_val = 1;
        for (int i = 0; i < d + 1; ++i)
            pow10_val *= (seq_t)10;

        /* build the number 3k7 */
        seq_t num = (seq_t)3 * pow10_val + (seq_t)cur_k * (seq_t)10 + (seq_t)7;

        /* test primality */
        if (is_prime_big(num)) {
            /* ensure cache capacity */
            if (found >= cap) {
                size_t new_cap = cap * 2 + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap   = new_cap;
            }
            cache[found] = (seq_t)cur_k;
            ++found;
        }
        ++cur_k;
    }

    return cache[found - 1];
}
