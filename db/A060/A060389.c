/*
 * A060389  a(1)=p_1, a(2)=p_1 + p_1*p_2, a(3)=p_1 + p_1*p_2 + p_1*p_2*p_3,
 *          ... where p_i is the i-th prime.
 * https://oeis.org/A060389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} ∏_{i=1..k} p_i (partial sums of primorials)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   ensure_capacity, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_up_to = 0;
    static seq_t primorial = 1;      /* product of primes up to cached_up_to */

    /* Return from cache if already known */
    if (n <= cached_up_to) {
        return cache[n - 1];
    }

    /* Ensure enough memory for terms up to n */
    ensure_capacity((size_t)n, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Compute new terms from (cached_up_to+1) to n */
    for (idx_t k = cached_up_to + 1; k <= n; ++k) {
        uint32_t pk = get_prime(k);           /* k-th prime */
        primorial = primorial * (seq_t)pk;    /* update primorial */

        seq_t val = (k == 1) ? primorial      /* for n==1 a(1)=p1 */
                            : cache[k - 2] + primorial; /* a(k)=a(k-1)+primorial */

        cache[k - 1] = val;
    }

    cached_up_to = n;
    return cache[n - 1];
}
