/*
 * A162214  a(n) = the largest positive multiple of n with exactly n digits
 *          when written in binary.
 * https://oeis.org/A162214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n * floor((2^n-1)/n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/*
 * Computes a(n) = the largest positive multiple of n with exactly n binary
 * digits.  The formula is a(n) = n * floor((2^n-1)/n), which is
 * equivalent to (2^n-1) - ((2^n-1) mod n).  We build 2^n by repeated
 * multiplication by 2; this works for any seq_t (fixed‑size or arbitrary
 * precision) as long as normal signed/unsigned overflow semantics apply
 * for the chosen seq_t type.
 */
seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic with seq_t values */
    seq_t N = (seq_t)n;

    /* Compute 2^n by repeated multiplication by 2 */
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        power *= (seq_t)2;
    }

    /* 2^n - 1 */
    seq_t val = power - (seq_t)1;

    /* remainder of (2^n - 1) modulo n */
    seq_t rem = val % N;

    /* Return (2^n - 1) - remainder, which equals n * floor((2^n-1)/n) */
    return val - rem;
}
