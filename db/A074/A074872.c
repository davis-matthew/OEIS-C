/*
 * A074872  Inverse BinomialMean transform of the Fibonacci sequence A000045
 *          (with the initial 0 omitted).
 * https://oeis.org/A074872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^floor((n-1)/2)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* exponent = floor((n-1)/2) */
    idx_t e = (n - 1) >> 1;          /* n >= 1, so n-1 is safe */
    seq_t result = 1;
    seq_t base = 5;

    while (e)
    {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}
