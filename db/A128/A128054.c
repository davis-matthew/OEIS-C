/*
 * A128054  List the nonnegative integers where multiples of 6 occur twice and
 *          numbers of the form 6k+4 are omitted.
 * https://oeis.org/A128054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*floor(n/6) if n mod 6 = 0 or 1 * a(n) = 6*floor(n/6) +
 *            1 if n mod 6 = 2 * a(n) = 6*floor(n/6) + 2 if n mod 6 = 3 * a(n)
 *            = 6*floor(n/6) + 3 if n mod 6 = 4 * a(n) = 6*floor(n/6) + 5 if n
 *            mod 6 = 5
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t block = n / 6;
    idx_t pos   = n % 6;
    seq_t base  = (seq_t)block * (seq_t)6;

    if (pos < 2)
        return base;
    else if (pos == 2)
        return base + (seq_t)1;
    else if (pos == 3)
        return base + (seq_t)2;
    else if (pos == 4)
        return base + (seq_t)3;
    /* pos == 5 */
    return base + (seq_t)5;
}
