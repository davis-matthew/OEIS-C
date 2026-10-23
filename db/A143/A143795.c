/*
 * A143795  a(1) = 1, then for n > 1, a(n) = a(n - 1) + 1 for n even, or a(n)
 *          = a(n - 1) + 10 for n odd.
 * https://oeis.org/A143795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (22*n -27 - 9*(-1)^n)/4
 * Range:     u8 n <= 48, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for safety return 0 for n = 0. */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    if (n & 1ULL)          /* odd n */
        return ((seq_t)11 * nn - 9) / 2;
    else                   /* even n */
        return ((seq_t)11 * nn - 18) / 2;
}
