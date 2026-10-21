/*
 * A069573  Largest n-digit number in which the k-th digit is a divisor or a
 *          multiple of k.
 * https://oeis.org/A069573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = integer whose k-th digit from the right (k≥1) is the
 *            largest digit d∈{1,…,9} dividing k or such that k divides d
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This can be computed greedily: for each position k from 1 to n
   choose the maximum admissible digit, then form the number by
   successive left‑shifts and additions. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = n; k > 0; --k) {
        int digit = 1;                     /* minimal admissible digit */
        for (int d = 9; d >= 1; --d) {     /* try largest to smallest */
            if (k % (idx_t)d == 0 || (idx_t)d % k == 0) {
                digit = d;
                break;
            }
        }
        result = result * 10 + digit;
    }
    return result;
}
