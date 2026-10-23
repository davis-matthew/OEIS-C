/*
 * A186194  a(n) = A002275(n) * (A002275(n)+1).
 * https://oeis.org/A186194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002275(n) * (A002275(n)+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            where A002275(n) = (10^n - 1)/9 (the repunit of length n) */
#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^n */
    seq_t p  = pow10(n);
    /* diff = 10^n - 1 */
    seq_t diff = p - 1;
    /* repunit(n) = (10^n - 1) / 9 */
    const seq_t nine = 9;
    seq_t rep = diff / nine;
    /* a(n) = rep * (rep + 1) */
    return rep * (rep + 1);
}
