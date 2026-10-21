/*
 * A060400  In base ten, we try to get the digits 0,1,2,3,...,9,0,1,2,3,... in
 *          order only using a sequence of increasing primes and each time
 *          choosing the smallest prime that will give the desired digit.
 * https://oeis.org/A060400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is defined by a greedy selection of increasing primes that
 *            collectively contain the digits 0,1,2,3,4,5,6,7,8,9 cyclically.
 *            For each i>=0 let's let d = i mod 10. a(i) is the smallest prime
 *            larger than a(i‑1) (with a(−1)=0) that contains the digit d in
 *            its decimal representation
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   grow_seq, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/grow_seq.h"

/* helper: does n contain digit d ? */
static bool contains_digit(uint64_t n, uint64_t d)
{
    if (n == 0) return d == 0;
    while (n > 0) {
        if ((n % 10) == d) return true;
        n /= 10;
    }
    return false;
}

/* the main function */
seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;   /* cached sequence values */
    static size_t cache_len = 0;     /* number of computed values */
    static size_t cache_cap = 0;     /* capacity of cache array */

    /* if we need more values, extend the cache and compute them */
    if (n >= cache_len) {
        size_t need = n + 1;
        if (need > cache_cap)
            grow_seq(&cache, &cache_cap, need);

        for (size_t i = cache_len; i < need; ++i) {
            uint64_t digit = i % 10;          /* desired digit */
            uint64_t prev  = (i == 0) ? 0
                                       : (uint64_t)cache[i - 1];
            uint64_t cand = prev + 1;          /* start searching */

            while (1) {
                /* test primality and the digit condition */
                if (is_prime_small((seq_t)cand) && contains_digit(cand, digit))
                    break;                    /* found the prime for this i */
                ++cand;
            }
            cache[i] = (seq_t)cand;
        }
        cache_len = need;
    }
    return cache[n];
}
