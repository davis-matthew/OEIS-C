/*
 * A098745  Number of primes between prime(n)/sqrt(2) and prime(n)*sqrt(2).
 * https://oeis.org/A098745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{primes p_i : prime(n)/√2 ≤ p_i ≤ prime(n)·√2}
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   compute_pi, get_prime, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/isqrt_seq.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* nth prime */
    seq_t p = (seq_t)get_prime(n);

    /* p², lower and upper bounds in squared terms */
    seq_t p2 = p * p;
    seq_t lower_sq = (p2 + (seq_t)1) / 2;   /* ceil(p² / 2)   */
    seq_t upper_sq = p2 * 2;                /* 2·p²           */

    /* smallest integer ≥ sqrt(lower_sq) */
    seq_t lower_sqrt = isqrt_seq(lower_sq);
    if (lower_sqrt * lower_sqrt < lower_sq)
        ++lower_sqrt;

    /* largest integer ≤ sqrt(upper_sq) */
    seq_t upper_sqrt = isqrt_seq(upper_sq);

    /* π(upper_sqrt) – π(lower_sqrt-1) */
    seq_t up = compute_pi((idx_t)upper_sqrt);
    seq_t lo = compute_pi((idx_t)(lower_sqrt - (seq_t)1));

    return up - lo;
}
