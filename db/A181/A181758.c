/*
 * A181758  Greatest prime closest to n^3.
 * https://oeis.org/A181758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the greatest prime closest to n^3; if two primes are
 *            equally close, return the larger one
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_prime_small.h"

/* local helper that chooses the most efficient primality test */
static bool is_prime_val(seq_t x)
{
    /* if x fits in uint64_t, use the small‑prime test */
    if (x <= (seq_t)UINT64_MAX)
        return is_prime_small((uint64_t)x);
    /* otherwise fall back to the big‑integer test */
    return is_prime_big(x);
}

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t target = tn * tn * tn;        /* n³ */

    for (idx_t d = 0; ; ++d) {
        /* check the upper side first (this guarantees tie → upper) */
        seq_t upper = target + (seq_t)d;
        if (is_prime_val(upper)) {
            if (d > 0 && target >= (seq_t)d) {
                seq_t lower = target - (seq_t)d;
                if (is_prime_val(lower))
                    return upper;      /* tie – choose the greater prime */
            }
            return upper;              /* prime found */
        }

        /* lower side (after d>0, since d==0 already handled by upper) */
        if (d > 0 && target >= (seq_t)d) {
            seq_t lower = target - (seq_t)d;
            if (is_prime_val(lower))
                return lower;          /* only lower prime found */
        }
    }
}
