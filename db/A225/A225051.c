/*
 * A225051  Numbers of the form x^3 + SumOfCubedDigits(x).
 * https://oeis.org/A225051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^3 + Sum_of_cubed_digits(n)
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the cube of each decimal digit of n and sum the results. */
static seq_t sum_of_cubed_digits(idx_t n)
{
    seq_t sum = 0;
    /* work with the index type; its range is uint64_t */
    uint64_t m = (uint64_t)n;
    while (m > 0)
    {
        unsigned d = (unsigned)(m % 10u);
        seq_t d_cube = (seq_t)d * d * d;
        sum += d_cube;
        m /= 10u;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t cube = sn * sn * sn;      /* n^3 */
    seq_t sum_digits_cube = sum_of_cubed_digits(n);  /* Sum of cubed digits of n */
    return cube + sum_digits_cube;
}
