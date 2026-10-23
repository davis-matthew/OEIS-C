/*
 * A048702  Binary palindromes of even length divided by 3.
 * https://oeis.org/A048702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A048701(n)/3. Conjecture (valid for n >= 1): a(n) =
 *            n*2^⌊log₂ n⌋ - Σ_{i=1}^{⌊log₂ n⌋} 2^(⌊log₂ n⌋ - i) * ⌊n/2^i⌋
 *            a(0) = 0
 * Range:     u8 n <= 23, u16 n <= 383, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(log₂(x)) for x > 0. */
static idx_t floor_log2_u64(idx_t x)
{
    idx_t k = 0;
    while (x >>= 1) {
        ++k;
    }
    return k;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* k = ⌊log₂ n⌋ */
    idx_t k = floor_log2_u64(n);

    /* power = 2^k */
    seq_t power = (seq_t)1 << k;

    /* result = n * 2^k */
    seq_t result = (seq_t)n * power;

    /* Subtract Σ_{i=1}^k 2^(k-i) * floor(n/2^i) */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t term = ((seq_t)1 << (k - i)) * ((seq_t)(n >> i));
        result -= term;
    }

    return result;
}
