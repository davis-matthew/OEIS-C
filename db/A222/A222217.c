/*
 * A222217  In the number n, replace all (decimal) digits '0' with '8' and
 *          vice versa.
 * https://oeis.org/A222217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is n with decimal digits 0 and 8 swapped
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    if (x == 0)
        return (seq_t)8;   /* a(0) = 8 */

    seq_t result = 0;
    seq_t mult    = 1;
    while (x > 0) {
        unsigned digit = (unsigned)(x % 10);        /* current decimal digit */
        unsigned swapped = (digit == 0 ? 8 : (digit == 8 ? 0 : digit));
        result += (seq_t)swapped * mult;
        mult   *= 10;
        x      /= 10;
    }
    return result;
}
