/*
 * A104340  Numbers n such that (digital reversal of n) - n = 9.
 * https://oeis.org/A104340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(n) = 11*n + 1 (for 1 ≤ n ≤ 8; for larger n the value does not
 *            exist)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Any call with n ≤ 0 is out of range; returning 0 is harmless. */
    if (n == 0)
        return 0;

    /* The sequence contains only eight terms: 12, 23, 34, 45, 56, 67, 78, 89. */
    if (n > 8)
        return 0;

    /* Compute using seq_t to respect the type constraints. */
    seq_t sn = (seq_t)n;
    return sn * 11 + 1;
}
