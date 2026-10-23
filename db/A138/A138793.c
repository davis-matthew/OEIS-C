/*
 * A138793  a(n) = concatenation of reversed digits of natural numbers from n
 *          down to 1.
 * https://oeis.org/A138793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concat_{k=n}^{1} rev(k) (rev(k) is k with its decimal
 *            digits reversed)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = n; k > 0; --k)
    {
        idx_t t = k;
        while (t > 0)
        {
            seq_t d = t % 10;
            result = result * 10 + d;
            t /= 10;
        }
    }
    return result;
}
