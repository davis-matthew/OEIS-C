/*
 * A122933  a(n)-th prime is equal to the sum_{i=1..k} pi(i) for some k (cf.
 *          A000720).
 * https://oeis.org/A122933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = PrimePi[ A122516[n] ] A122516[n] is the n-th prime that
 *            occurs as a cumulative sum of π(k). This implementation
 *            iteratively generates π(m), keeps the running sum
 *            S(m)=∑_{i=1..m}π(i), and whenever S(m) is prime records its
 *            prime‑index using compute_pi (PrimePi). Results are memoised so
 *            call...
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   compute_pi, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/compute_pi.h"   /* provides compute_pi(idx_t) */
#include "../utils/is_prime.h"     /* provides is_prime(idx_t) */
#include "../utils/grow_seq.h"     /* provides grow_seq(seq_t **, size_t *, size_t) */

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* State of the incremental generator */
    static idx_t cur_m = 0;          /* last value of m processed   */
    static idx_t cur_pi = 0;         /* π(m) at cur_m               */
    static seq_t cur_sum = 0;        /* S(m) = ∑_{i=1..m}π(i)      */

    /* Fast return if the requested term was already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Extend the cache until we have at least n terms */
    while (cache_len < n)
    {
        /* Advance to the next integer */
        cur_m++;

        /* Update π(m): it increases by 1 if cur_m is prime */
        if (is_prime(cur_m))
            cur_pi++;

        /* Update the cumulative sum S(m) */
        cur_sum += cur_pi;

        /* If S(m) is prime, it belongs to A122516 */
        if (is_prime((idx_t)cur_sum))
        {
            /* a(n) = prime index of S(m) = π(S(m)) */
            seq_t term = compute_pi((idx_t)cur_sum);

            /* Ensure capacity for the new term */
            if (cache_len == cache_cap)
                grow_seq((seq_t **)&cache, &cache_cap, cache_len + 1);

            cache[cache_len++] = term;
        }
    }

    return cache[n - 1];
}
