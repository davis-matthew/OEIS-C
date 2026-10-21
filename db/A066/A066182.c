/*
 * A066182  Permutation of the integers with cycle form {1}, {3, 2}, {6, 5,
 *          4}, {10, 9, 8, 7}, ...
 * https://oeis.org/A066182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = n-1 + binom(round(sqrt(2*n))+1,2) -
 *            binom(round(sqrt(2*(n-1)))+1,2)
 * Range:     u8 n <= 253, u16 n <= 9508, u32 n <= 9508, u64 n <= 9508,
 *            u128 n <= 9508, big any n
 * Helpers:   binom2, floor_sqrt_of_2n, round_sqrt_of_2n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation avoids floating point and integer overflow by using 128‑bit arithmetic. */

#include <stdint.h>
#include "../utils/binom2.h"
#include "../utils/floor_sqrt_of_2n.h"
#include "../utils/round_sqrt_of_2n.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    idx_t t = round_sqrt_of_2n(n);           /* A002024(n)   */
    idx_t u = round_sqrt_of_2n(n - 1);       /* A002024(n-1) */

    /* binom(round(sqrt(2*n))+1,2) = binom(t+1,2) */
    seq_t binot = binom2((seq_t)(t + 1));
    seq_t binou = binom2((seq_t)(u + 1));

    /* a(n) = n-1 + binot - binou  (all operations in seq_t) */
    return (seq_t)n - 1 + binot - binou;
}
