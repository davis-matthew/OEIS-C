/*
 * A072970  Least k > 0 such that the last digit of k^n is the same as the
 *          last digit of n*k.
 * https://oeis.org/A072970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   pow_mod10_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least k>0 such that the last digit of k^n equals the last
   digit of n*k. Uses a simple brute‑force search. */

#include <stdint.h>
#include "../utils/pow_mod10_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for offset < 1 */
    uint64_t n10 = (uint64_t)(n % 10);
    for (idx_t k = 1; ; ++k)
    {
        uint64_t kn10 = (n10 * (k % 10)) % 10;
        uint64_t kp10 = pow_mod10_seq(k, n);
        if (kn10 == kp10) return (seq_t)k;
    }
}
