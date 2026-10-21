/*
 * A090350  a(n) = Floor[(n-PrimePi[n])/(2-PrimePi[n]+PrimePi[n-1])].
 * https://oeis.org/A090350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor[(n - PrimePi[n]) / (2 - PrimePi[n] + PrimePi[n-1])]
 *            (offset 1)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    seq_t pi_n   = prime_pi(n);                 /* π(n)  */
    seq_t pi_nm1 = (n == 0) ? 0 : prime_pi(n - 1); /* π(n‑1) */
    seq_t num = ((seq_t)n) - pi_n;
    seq_t den = (seq_t)2 - pi_n + pi_nm1;
    return num / den;
}
