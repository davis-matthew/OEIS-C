/*
 * A049559  a(n) = gcd(n - 1, phi(n)).
 * https://oeis.org/A049559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n - 1, phi(n))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A049559: a(n) = gcd(n-1, φ(n)). */

#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* For n >= 1 (offset 1). */
    seq_t nm1 = (seq_t)n - 1;          /* compute n-1 in seq_t */
    seq_t phi_n = phi(n);              /* φ(n) using provided routine */
    return gcd(nm1, phi_n);           /* gcd of the two values */
}
