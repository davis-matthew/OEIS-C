/*
 * A026035  Expansion of x^2*(2 - x + x^2) / ((1 + x)*(1 - x)^4).
 * https://oeis.org/A026035
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)/6 + (n*n)/2 for n >= 2
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t bin = nn * (nn - 1) * (nn - 2) / 6;
    seq_t sqpart = nn * nn / 2;
    return bin + sqpart;
}
