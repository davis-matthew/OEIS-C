/*
 * A033036  Numbers all of whose base 11 digits are odd.
 * https://oeis.org/A033036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑11 digits are all
 *            odd
 * Range:     u8 n <= 55, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the length k of the desired number */
    seq_t pow5 = 5;        /* 5^1 */
    seq_t cumulative = 5;  /* Σ_{i=1}^{1} 5^i */
    idx_t k = 1;
    while ((seq_t)n > cumulative) {
        pow5 *= 5;
        cumulative += pow5;
        ++k;
    }

    /* cumulative holds Σ_{i=1}^{k} 5^i, pow5 = 5^k */
    seq_t cumulative_prev = cumulative - pow5;   /* Σ_{i=1}^{k-1} 5^i */

    /* offset of the term within the block of k‑digit numbers (0‑based) */
    seq_t offset = (seq_t)n - cumulative_prev - 1;

    /* Convert offset to k base‑5 digits (most significant first) */
    int digits[64];
    seq_t tmp = offset;
    for (idx_t i = k; i > 0; --i) {
        digits[i - 1] = (int)(tmp % 5);
        tmp /= 5;
    }

    /* Build the base‑11 number from these digits */
    seq_t result = 0;
    for (idx_t i = 0; i < k; ++i) {
        seq_t d = (seq_t)(2 * digits[i] + 1);   /* 0→1, 1→3, 2→5, 3→7, 4→9 */
        result = result * 11 + d;
    }

    return result;
}
