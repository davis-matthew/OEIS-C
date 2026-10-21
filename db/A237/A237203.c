/*
 * A237203  Least positive integer k < n/2 with phi(k)*phi(n-k) a square, or 0
 *          if such a number k does not exist.
 * https://oeis.org/A237203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 850, u16 n <= 1764, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, isqrt_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least positive integer k < n/2 such that φ(k)·φ(n−k) is a perfect square;
   returns 0 if no such k exists */
#include "../utils/calc_phi.h"
#include "../utils/seq_mul.h"
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t result = 0;
    idx_t maxk = (n - 1) / 2;          /* floor((n-1)/2) */

    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t phi_k  = calc_phi((seq_t)k);
        seq_t phi_nk = calc_phi((seq_t)(n - k));
        seq_t prod   = seq_mul(phi_k, phi_nk);
        seq_t sq     = isqrt_seq(prod);
        if (seq_mul(sq, sq) == prod) {
            result = (seq_t)k;
            break;
        }
    }

    return result;
}
