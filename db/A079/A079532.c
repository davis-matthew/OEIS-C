/*
 * A079532  a(n) = floor(n - sqrt(n)) - phi(n).
 * https://oeis.org/A079532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 208, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   isqrt, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor(n - sqrt(n)) - φ(n)
 * Computes a(n) for n ≥ 1.
 * Uses an integer–only square‑root implementation and the pre‑provided
 * totient routine from utils/phi.h.
 */
#include "../utils/isqrt.h"
#include "../utils/phi.h"

/* Integer square root: floor(sqrt(n)) for 0 ≤ n < 2^64. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62;          /* largest power of 4 ≤ 2^64  */

    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* floor(sqrt(n)) */
    idx_t s = isqrt_u64(n);
    seq_t seqs = (seq_t)s;
    seq_t seqn = (seq_t)n;

    /* floor(n - sqrt(n)) */
    seq_t floor_term = seqn - seqs;
    if (seqs * seqs != seqn)        /* n not a perfect square */
        floor_term -= 1;

    /* φ(n) from utils/phi.h */
    seq_t phi_val = phi(n);         /* the function is defined in phi.h */

    return floor_term - phi_val;
}
