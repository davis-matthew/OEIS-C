/*
 * A078321  Euler's totient of n*rad(n)+1, where rad = A007947 (squarefree
 *          kernel).
 * https://oeis.org/A078321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(n · rad(n) + 1) where rad(n) is the squarefree kernel
 *            of n
 * Range:     u8 n <= 16, u16 n <= 264, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 998 (2026-11-01)
 * Helpers:   radical, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/radical.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    seq_t rn = radical(n);                       /* rad(n)  */
    seq_t mn = ((seq_t)n) * rn + (seq_t)1;   /* m = n·rad(n) + 1 */
    return totient_seq(mn);                     /* φ(m)   */
}
