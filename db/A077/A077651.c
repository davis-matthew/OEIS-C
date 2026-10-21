/*
 * A077651  Initial digit of phi(n), where phi is Euler totient function,
 *          A000010.
 * https://oeis.org/A077651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A000030(A000010(n)) – the first decimal digit of φ(n)
 * Range:     u8 n <= 462, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t val = phi(n);                 /* compute Euler's totient φ(n) */
    while (val >= 10)                    /* strip off trailing digits until one remains */
        val /= 10;
    return val;                          /* leading digit of φ(n) */
}
