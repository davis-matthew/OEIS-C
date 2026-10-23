/*
 * A126870  a(1)=1. a(n) = number of earlier terms of the sequence which are
 *          coprime to the sum of distinct prime divisors of n.
 * https://oeis.org/A126870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A126870 – OEIS sequence
 *
 *  Definition:
 *      a(1) = 1
 *      For n > 1:
 *          s = sum of distinct prime divisors of n
 *          a(n) = number of k in [1, n-1] with gcd(a(k), s) = 1
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Helper: sum of distinct prime divisors of n (1‑based index) */
static seq_t sum_distinct_prime_divisors(idx_t n)
{
    seq_t sum = 0;
    idx_t temp = n;
    for (idx_t p = 2; (idx_t)p * (idx_t)p <= temp; ++p) {
        if (temp % p == 0) {
            sum += (seq_t)p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        sum += (seq_t)temp;
    return sum;
}

/* Main sequence function with memoisation */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    size_t needed = (size_t)n;

    /* Extend cache if necessary */
    if (needed > cache_size) {
        /* Cast realloc return to avoid C++ implicit conversion error */
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache)  /* allocation failure – abort silently */
            exit(EXIT_FAILURE);
        cache = new_cache;

        /* Compute new terms */
        for (size_t i = cache_size; i < needed; ++i) {
            idx_t idx = (idx_t)(i + 1);          /* sequence index (1‑based) */
            if (idx == 1) {
                cache[i] = 1;
                continue;
            }
            seq_t s = sum_distinct_prime_divisors(idx);
            seq_t cnt = 0;
            for (size_t k = 0; k < i; ++k) {
                if (gcd(cache[k], s) == 1)
                    ++cnt;
            }
            cache[i] = cnt;
        }
        cache_size = needed;
    }

    return cache[n - 1];
}
