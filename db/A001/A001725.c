/*
 * A001725  a(n) = n!/5!.
 * https://oeis.org/A001725
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! / 5! = 6 * 7 * 8 * ... * n (product from i=6 to n)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 6; i <= n; i++) {
        result *= (seq_t)i;
    }
    return result;
}
