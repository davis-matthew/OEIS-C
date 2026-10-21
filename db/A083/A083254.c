/*
 * A083254  a(n) = 2*phi(n) - n.
 * https://oeis.org/A083254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*phi(n) – n, where phi is Euler’s totient
 * Range:     i8 n <= 129, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   my_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/my_phi.h"

/* OEIS sequence A083254 */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1 – values for n < 1 are undefined */
        return 0;

    seq_t phi = my_phi(n);
    seq_t two_phi = phi << 1;      /* 2 * phi(n) */
    return two_phi - (seq_t) n;    /* 2*phi(n) – n */
}
