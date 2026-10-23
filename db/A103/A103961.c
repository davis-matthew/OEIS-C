/*
 * A103961  Least k such that 2*n*k - 1 is a prime.
 * https://oeis.org/A103961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min{k ≥ 1 : 2*n*k – 1 is prime}
 * Range:     u8 n <= 42, u16 n <= 5538, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    /* n >= 1, but the formula works for any positive n */
    const seq_t n_seq = (seq_t)n;
    idx_t k = 1;
    for (;; ++k) {
        /* Compute p = 2*n*k – 1 using seq_t arithmetic */
        seq_t p = (seq_t)2 * n_seq * (seq_t)k - 1;
        if (is_prime_seq(p))
            return (seq_t)k;
    }
}
