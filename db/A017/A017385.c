/*
 * A017385  a(n) = (10*n + 9)^9.
 * https://oeis.org/A017385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 9)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 12, u128 n <= 1910,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (10*n + 9).  All arithmetic is done in seq_t
       to ensure correct behaviour for arbitrary-precision seq_t. */
    seq_t base = (seq_t)(10 * n) + (seq_t)9;

    /* Compute base^9 by repeated multiplication.  The exponent 9 is
       a compile‑time constant, so this loop is optimized out in many
       compilers. */
    seq_t result = base;
    for (idx_t i = 1; i < 9; ++i)
        result *= base;

    return result;
}
