/*
 * A098724  a(n) = C(n, 5)^(n-6).
 * https://oeis.org/A098724
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n,5)^(n-6) for n ≥ 6; a(5) = 1
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 10, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of sequence A098724.
   The function is valid for any build where seq_t is an unsigned integer
   of arbitrary size (8,16,32,64 or 128 bits, or an arbitrary‑precision
   integer class compiled as C++ 17). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset: the first listed term is a(5) = 1. */
    if (n <= 5) {
        if (n == 5) return (seq_t)1;
        /* Behaviour for n < 5 is not specified; return 0. */
        return (seq_t)0;
    }

    /* Compute binomial coefficient C(n,5) in a safe way
       to avoid intermediate overflow on 64‑bit builds. */
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= 5; ++i) {
        binom = (binom * (seq_t)(n - i + 1)) / (seq_t)i;
    }

    /* Exponent (n-6) is non‑negative since n ≥ 6. */
    idx_t exp = n - 6;
    seq_t result = (seq_t)1;

    /* Fast exponentiation by squaring. */
    while (exp > 0) {
        if (exp & 1) result = result * binom;
        exp >>= 1;
        if (exp) binom = binom * binom;
    }

    return result;
}
