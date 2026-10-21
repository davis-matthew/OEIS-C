/*
 * A223175  Smallest prime q such that 2n+1 = p + 8*q for some odd prime p, or
 *          0 if no such prime exists.
 * https://oeis.org/A223175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime q such that 2*n+1 = p + 8*q for some odd
 *            prime p, or 0 if no such prime exists
 * Range:     u8 n <= 934, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"       /* is_prime(idx_t)      */
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t)  */

/* Helper to shrink a seq_t value to idx_t when it fits. */
static inline bool seq_to_idx(seq_t x, idx_t *out)
{
    /* We rely on the fact that seq_t can be compared with integer
       constants.  If the value is too large for idx_t, or equals
       UINT64_MAX, we treat it as too large. */
    if (x <= (seq_t)UINT64_MAX) {
        *out = (idx_t)x;
        return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* 2*n + 1 (always odd) */
    seq_t target = ((seq_t)n) * 2 + 1;

    /* No odd prime p can satisfy the equation for target < 3. */
    if (target < 3)
        return (seq_t)0;

    /* Compute maximum possible q: 8*q <= target-3 => q <= (target-3)/8 */
    seq_t diff = target - 3;
    seq_t max_q_seq = diff / 8;

    /* Convert to idx_t if possible; otherwise there's no feasible q. */
    idx_t max_q;
    if (!seq_to_idx(max_q_seq, &max_q))
        return (seq_t)0;   /* target so large that q would not fit in idx_t */

    for (idx_t q = 2; q <= max_q; ++q) {
        /* q must be prime. */
        if (!is_prime(q))
            continue;

        /* Corresponding p = target - 8*q. */
        seq_t p = target - (seq_t)8 * q;

        /* p must be an odd prime at least 3. */
        if (p < 3)
            continue;

        /* Fast path: p is even and >2, skip immediately. */
        if ((p & 1) == 0 && p != 2)
            continue;

        /* Finally check primality of p. */
        if (is_prime_big(p))
            return (seq_t)q;  /* smallest qualifying q found */
    }

    /* No suitable prime q. */
    return (seq_t)0;
}
