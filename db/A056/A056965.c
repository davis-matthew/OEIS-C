/*
 * A056965  a(n) = n - (reversal of digits of n).
 * https://oeis.org/A056965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign, look
 * Formula:   a(n) = n - reverse_digits(n)
 * Range:     i8 n <= 102, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t reverse_uint64(idx_t n)
{
    idx_t rev = 0;
    while (n > 0)
    {
        rev = rev * 10 + (n % 10);
        n /= 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    idx_t rev = reverse_uint64(n);
    return (seq_t)n - (seq_t)rev;
}
