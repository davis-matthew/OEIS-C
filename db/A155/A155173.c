/*
 * A155173  Short leg A of primitive Pythagorean triangles such that perimeter
 *          s is average of twin prime pairs, q=p+1, A=q^2-p^2, C=q^2+p^2,
 *          B=2*p*q, s=A+B+C; s -/+ 1 are primes.
 * https://oeis.org/A155173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A155173: a(n) = the n-th odd integer k such that k^2+k-1
   and k^2+k+1 are both prime.  */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, a(1) is first term */
    seq_t count = 0;
    seq_t k = 3;                           /* first odd >1 */

    while (true) {
        seq_t k2 = k * k;                  /* k^2 */
        seq_t x1 = k2 + k - 1;             /* k^2 + k - 1 */
        seq_t x2 = k2 + k + 1;             /* k^2 + k + 1 */
        if (isPrimeGeneric(x1) && isPrimeGeneric(x2)) {
            count++;
            if (count == n) return k;
        }
        k += 2;                            /* next odd integer */
    }
}
