/*
 * A191554  a(n) = Product_{k=1..n} prime(k)^(2^(k-1)).
 * https://oeis.org/A191554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} prime(k)^(2^(k-1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, get_prime, seq_alloc_array, seq_free_array, seq_mul,
 *            seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* Compute a(n) for any n ≥ 1.  Results are memoised so
 * subsequent calls are fast, even if the call order is
 * non‑monotonic. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cache[i] holds a(i+1) */
    static idx_t cache_size = 0;       /* number of terms stored */

    if (n == 0)                         /* undefined; return 0 */
        return (seq_t)0;

    /* Extend cache if necessary */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = seq_alloc_array(new_size);
        /* copy existing values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        /* compute from cache_size+1 to n */
        seq_t result;
        if (cache_size == 0)           /* a(1) = prime(1) */
            result = seq_one();        /* will be multiplied below */
        else
            result = cache[cache_size - 1];

        /* exponent 2^(k-1) for the first new term */
        seq_t exp = seq_one();         /* 2^0 */
        for (idx_t k = 1; k < cache_size + 1; ++k) /* skip to new start */
            exp <<= 1;                 /* exp = 2^k */

        for (idx_t k = cache_size + 1; k <= n; ++k) {
            uint32_t pk = get_prime(k);           /* prime(k) */
            seq_t prime_val = (seq_t)pk;
            seq_t term = big_pow(prime_val, exp); /* prime^(2^(k-1)) */
            result = seq_mul(result, term);      /* accumulate product */
            new_cache[k - 1] = result;           /* store a(k) */
            exp <<= 1;                           /* prepare next exponent */
        }
        /* free old cache and replace */
        if (cache)
            seq_free_array(cache);
        cache = new_cache;
        cache_size = new_size;
    }
    return cache[n - 1];
}
