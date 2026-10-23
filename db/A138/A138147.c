/*
 * A138147  Concatenation of n digits 1 and n digits 0.
 * https://oeis.org/A138147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = (10^(2*n) - 10^n)/9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* sequence is defined for n ≥ 1 */

    /* Compute 10^n as seq_t */
    seq_t pow10n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow10n *= (seq_t)10;

    /* Number consisting of n ones: (10^n - 1) / 9 */
    seq_t ones = pow10n;
    ones -= (seq_t)1;
    ones /= (seq_t)9;

    /* Concatenate the ones with n zeros: ones * 10^n */
    return ones * pow10n;
}
