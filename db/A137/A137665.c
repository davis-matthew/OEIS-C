/*
 * A137665  Quotients ((p+1)^p - 1)/p^2 for p = prime(n).
 * https://oeis.org/A137665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((prime(n) + 1)^prime(n) - 1)/(prime(n)^2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   compute_with_uint128, get_prime, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t) */
#include "../utils/seq_pow.h"    /* seq_pow(seq_t, idx_t) */
#include "../utils/seq_one.h"   /* seq_one(void) */
#include "../utils/compute_with_uint128.h"

seq_t a(idx_t n) {
    /* fetch the n-th prime (1‑indexed) */
    uint32_t p = get_prime(n);

    /* If seq_t is a 8‑byte unsigned integer, use a 128‑bit temporary
       to avoid overflow for the small values that actually fit in
       64‑bit.   For larger primes we fall back to the generic
       seq_pow, but those values no longer fit in seq_t and are not
       required to be correct.                                */
    if (sizeof(seq_t) <= 8) {
        if (p <= 17) {           /* 17 gives the largest 64‑bit result */
            return compute_with_uint128(p);
        } else {
            /* use the generic big‑int routine, although it will
               overflow for these large primes.  The results for
               them do not need to be correct in a 64‑bit build. */
            seq_t base = (seq_t)(p + 1);
            seq_t pow_val = seq_pow(base, (idx_t)p);
            seq_t numerator = pow_val - seq_one();
            seq_t denom = (seq_t)p * (seq_t)p;
            return numerator / denom;
        }
    }

    /* For arbitrary‑precision seq_t the generic routine works. */
    seq_t base = (seq_t)(p + 1);
    seq_t pow_val = seq_pow(base, (idx_t)p);
    seq_t numerator = pow_val - seq_one();
    seq_t denom = (seq_t)p * (seq_t)p;
    return numerator / denom;
}
