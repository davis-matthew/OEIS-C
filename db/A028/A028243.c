/*
 * A028243  a(n) = 3^(n-1) - 2^n + 1 (essentially Stirling numbers of second
 *          kind).
 * https://oeis.org/A028243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-1) - 2^n + 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the arbitrary‑precision power routine provided by the build system. */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For n < 1 the offset is not defined; return 0 for safety */
    if (n == 0) return 0;

    /* Compute the required powers of 3 and 2 */
    seq_t p3 = big_pow((seq_t)3, (seq_t)(n - 1)); /* 3^(n-1) */
    seq_t p2 = big_pow((seq_t)2, (seq_t)n);       /* 2^n */

    /* a(n) = 3^(n-1) - 2^n + 1 */
    return p3 - p2 + (seq_t)1;
}
