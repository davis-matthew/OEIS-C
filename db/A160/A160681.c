/*
 * A160681  Primes of form 10 * 2^(2^n) + 19.
 * https://oeis.org/A160681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = 10 * 2^(2^n) + 19
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, mul_small, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#include "../utils/big_pow.h"
#include "../utils/pow_int.h"
#include "../utils/mul_small.h"

/* Compute the nth term of OEIS sequence A160681.
   n is 1‑based: a(1) = 59, a(2) = 179, ... */
seq_t a(idx_t n)
{
    /* memoization cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* extend cache to accommodate n terms */
    if (n > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms if needed */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i is 0‑based index, so exponent is n = i+1 */
        seq_t exp_val = pow_int(2, i + 1);          /* 2^(i+1)   */
        seq_t val = big_pow(2, exp_val);            /* 2^(2^(i+1)) */
        val = mul_small(val, 10);                   /* multiply by 10 */
        val += 19;                                  /* add 19     */
        cache[i] = val;
    }
    cache_len = n;

    return cache[n - 1];
}
