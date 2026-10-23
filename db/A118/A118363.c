/*
 * A118363  Factorial base Niven (or Harshad) numbers: numbers that are
 *          divisible by the sum of their factorial base digits.
 * https://oeis.org/A118363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth factorial base Niven (Harshad) number. The function
 *            finds the nth positive integer whose factorial‑base expansion is
 *            divisible by the sum of its factorial‑base digits by brute‑force
 *            search
 * Range:     u8 n <= 54, u16 n <= 5565, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_factorial_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sum_factorial_digits.h"

seq_t a(idx_t n)
{
    /* Sequentially search candidate integers */
    idx_t   found = 0;
    seq_t   cand  = (seq_t)1;

    while (found < n) {
        seq_t s = sum_factorial_digits(cand);
        if (s != 0 && (cand % s) == 0) {
            found++;
            if (found == n)
                return cand;
        }
        cand++;
    }

    /* Unreachable, but keeps the compiler happy */
    return (seq_t)0;
}
