/*
 * A124363  a(n) = n^3 + 71*n + 1.
 * https://oeis.org/A124363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)^3 + 71*(n-1) + 1, offset 1
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined terms for n = 0; return 0 as a safe default. */
    if (n == 0)
        return (seq_t)0;

    /* Compute m = n-1 in the integer type used by the target build. */
    seq_t m = (seq_t)(n - 1);

    /* Apply the definition: a(n) = m^3 + 71*m + 1. */
    return m * m * m + (seq_t)71 * m + (seq_t)1;
}
