/*
 * A110748  Form triangle shown below, in which the n-th row contains n terms
 *          of an arithmetic progression with first term 1 and common
 *          difference n. Then a(n) = terms of the n-th row (mod 10),
 *          concatenated.
 * https://oeis.org/A110748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = concatenation of (1 + k*n mod 10) for k = 0 … n-1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)0;
    unsigned d = 1;                 /* first digit: 1 */
    const seq_t ten = (seq_t)10;
    idx_t i;

    for (i = 0; i < n; ++i) {
        res = res * ten + (seq_t)d;
        d = (d + (unsigned)n) % 10; /* next digit: (current + n) mod 10 */
    }
    return res;
}
