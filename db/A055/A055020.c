/*
 * A055020  Number of iterations of sigma() required until 2n (or greater) is
 *          reached.
 * https://oeis.org/A055020
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k such that σ^k(n) ≥ 2n, where σ(n) is the
 *            sum of divisors of n
 * Range:     u8 n <= 66, u16 n <= 9858, u32 n <= 9858, u64 n <= 9858,
 *            u128 n <= 9858, big any n
 * Helpers:   sigma_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_val.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* behaviour below offset is irrelevant */

    seq_t target = (seq_t)n * 2;   /* 2n */
    seq_t current = (seq_t)n;
    seq_t count = (seq_t)0;

    while (current < target) {
        current = sigma_val((idx_t)current);
        count++;
    }
    return count;
}
