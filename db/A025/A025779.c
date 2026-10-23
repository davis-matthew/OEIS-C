/*
 * A025779  Expansion of 1/((1-x)*(1-x^5)*(1-x^9)).
 * https://oeis.org/A025779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{c=0}^{⌊n/9⌋} (⌊(n-9c)/5⌋ + 1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n using parts 1, 5, and 9. */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_c = n / 9;
    for (idx_t c = 0; c <= max_c; ++c)
    {
        idx_t rem = n - 9 * c;
        idx_t bmax = rem / 5;
        sum += (seq_t)(bmax + 1);
    }
    return sum;
}
