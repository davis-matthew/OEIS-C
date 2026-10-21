/*
 * A089226  Smallest composite number with exactly n 1's in binary
 *          representation.
 * https://oeis.org/A089226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest composite number with exactly n 1's in binary
 *            representation. The function iterates over the integers with
 *            popcount n in increasing order using the classic algorithm to
 *            obtain the next integer with the same number of 1‑bits. It stops
 *            when a composite value is found
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 16, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* First candidate: the smallest number with n ones is 2^n−1. */
    seq_t val = pow2(n);
    val -= (seq_t)1;

    /* Search until a composite is found. */
    while (!((val > (seq_t)1) && !is_prime_big(val))) {
        /* next = next integer with the same popcount as val.
           Algorithm from
              Gosper's hack: lowbit = val & (~val + 1)
              next = val + lowbit
              right = ((val ^ next) >> 2) / lowbit
              val = next | right   */
        seq_t lowbit = val & (~val + (seq_t)1);  /* lowest set bit */
        seq_t next_val = val + lowbit;
        seq_t right = (val ^ next_val) >> 2;
        right /= lowbit;
        val = next_val | right;
    }

    return val;
}
