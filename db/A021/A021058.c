/*
 * A021058  Decimal expansion of 1/54.
 * https://oeis.org/A021058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th decimal digit of 1/54. Computed by successive
 *            long division: a(n) = (10^(n+1)/54) % 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long rem = 1;   /* remainder of 1 divided by 54 */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;            /* shift a decimal digit left */
        digit = (seq_t)(rem / 54ULL);
        rem   =  rem % 54ULL;
    }
    return digit;
}
