/*
 * A174257  Number of symmetry classes of 3 X 3 reduced magic squares with
 *          distinct values and maximum value 2n; also, with magic sum 3n.
 * https://oeis.org/A174257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/2) + floor((n-1)/3) - floor(n/3)
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* sequence offset starts at n = 1 */

    idx_t t1 = (n - 1) / 2;
    idx_t t2 = (n - 1) / 3;
    idx_t t3 = n / 3;

    return (seq_t)(t1 + t2 - t3);
}
