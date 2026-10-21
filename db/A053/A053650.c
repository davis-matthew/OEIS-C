/*
 * A053650  Cototient function of n^2.
 * https://oeis.org/A053650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = n * (n - phi(n)) (cototient of n²)
 * Range:     u8 n <= 21, u16 n <= 299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/phi_small.h"

/* Main function required by the build system */
seq_t a(idx_t n)
{
    /* Guard against the offset 1: a(1)=0 trivially */
    if (n < 1) {
        return (seq_t)0;
    }

    seq_t phi_n = phi_small(n);          /* φ(n) */
    seq_t un   = (seq_t)n;               /* cast index to seq_t  */
    return un * (un - phi_n);            /* a(n) = n*(n - φ(n)) */
}
