/*
 * A070045  Primes of the form ceiling(k^1.5) for some integer k.
 * https://oeis.org/A070045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form ceil(k^1.5), k ∈ ℕ (offset 1)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 73, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/append_candidate.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *primes = NULL;
    static size_t len = 0, cap = 0;
    static idx_t last_k = 1;          /* next k to test */

    /* target number of primes to have */
    size_t target = (size_t)n;

    /* generate primes until we have enough */
    while (len < target)
    {
        idx_t k = last_k++;
        seq_t kseq = (seq_t)k;

        /* k^3 */
        seq_t k_sq  = kseq * kseq;   /* k^2 */
        seq_t k_cub = k_sq * kseq;   /* k^3 */

        /* ceil(sqrt(k^3)) = ceil(k^1.5) */
        seq_t t = floor_sqrt(k_cub); /* floor sqrt */
        if (t * t < k_cub)          /* not exact square, must add 1 */
            ++t;

        /* check primality and record */
        if (is_prime_big(t))
            append_candidate(&primes, &len, &cap, t);
    }

    /* return the requested element (offset 1) */
    return primes[target - 1];
}
