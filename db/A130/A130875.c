/*
 * A130875  Absolute difference of final digits of two consecutive cubes.
 * https://oeis.org/A130875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = |(n³ mod 10) - ((n-1)³ mod 10)| (n ≥ 1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t cubic_last[10] = {0, 1, 8, 7, 4, 5, 6, 3, 2, 9};

    /* last digit of n³ and (n-1)³ */
    seq_t d1 = cubic_last[n % 10];
    seq_t d0 = cubic_last[(n - 1) % 10];

    /* absolute difference */
    return (d1 >= d0) ? (d1 - d0) : (d0 - d1);
}
