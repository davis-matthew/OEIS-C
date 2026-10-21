/*
 * A202606  a(n) = ceiling(((10^n - 1)^2/9 + 10^n)/18).
 * https://oeis.org/A202606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((10^n – 1)/9) * ((10^n – 1)/9 + 1)/2 + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10, tri_half_carry
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A202606.
 * Uses the provided pow10() utility to obtain 10^n as a seq_t value.
 * Then evaluates
 *   k = (10^n – 1)/9
 *   a = k*(k+1)/2 + 1
 * which is mathematically equivalent to the original definition but
 * uses only integer arithmetic on seq_t. */
 
#include "../utils/pow10.h"   /* provides seq_t pow10(idx_t e) */
#include "../utils/tri_half_carry.h"

seq_t a(idx_t n)
{
    /* 10^n as seq_t */
    seq_t p = pow10(n);

    /* k = (10^n – 1)/9 */
    seq_t one = (seq_t)1;
    seq_t k   = (p - one) / (seq_t)9;

    /* a = k*(k+1)/2 + 1 */
    seq_t kp1  = k + one;
    seq_t prod = tri_half_carry(k, kp1);
    seq_t res  = prod + one;

    return res;
}
