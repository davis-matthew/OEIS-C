/*
 * A065137  Sum of digits of n plus sum of cubes of digits of n.
 * https://oeis.org/A065137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of digits of n + sum of cubes of digits of n
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum_digits = 0;
    seq_t sum_cubes  = 0;
    idx_t t = n;
    while (t != 0)          /* handle n == 0 correctly: loop skipped */
    {
        uint64_t d = t % 10;  /* extract least‑significant decimal digit */
        t /= 10;
        sum_digits += (seq_t)d;
        seq_t dd = (seq_t)d;
        sum_cubes += dd * dd * dd;   /* d³ */
    }
    return sum_digits + sum_cubes;
}
