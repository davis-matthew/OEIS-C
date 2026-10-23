/*
 * A159733  Number of permutations of 5 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 1 local maximum.
 * https://oeis.org/A159733
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n*6^(n-2) (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp using exponentiation by squaring.
   All arithmetic is performed in seq_t, which may be an
   unsigned integer or an arbitrary‑precision integer class. */
static seq_t seq_pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* OEIS sequence A159733: Number of permutations of 5 indistinguishable copies
   of 1..n arranged in a circle with exactly 1 local maximum. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2. For smaller n the value
       is not specified; returning 0 is harmless. */
    if (n < 2)
        return 0;

    /* Compute 6^(n-2) in seq_t. */
    seq_t pow6 = seq_pow_seq_t((seq_t)6, n - 2);

    /* Compute 5 * n in seq_t. */
    seq_t five_n = (seq_t)5 * (seq_t)n;

    /* Final result: (5*n) * 6^(n-2). */
    return pow6 * five_n;
}
