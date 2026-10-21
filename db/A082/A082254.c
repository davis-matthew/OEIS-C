/*
 * A082254  Concatenation of (6n-5), (6n-4), (6n-3), (6n-2), (6n-1) and 6n
 *          divided by 6.
 * https://oeis.org/A082254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(6n-5, 6n-4, 6n-3, 6n-2, 6n-1, 6n) / 6
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 166, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)6 * (seq_t)n;          /* 6n as seq_t */
    seq_t res = 0;

    for (idx_t j = 1; j <= 6; ++j)            /* j = 1 .. 6   */
    {
        /* term = 6n - (6 - j) = 6n - 5, -4, ..., 0 */
        seq_t term = base - (seq_t)(6 - j);

        /* count decimal digits of term */
        seq_t tmp = term;
        seq_t pow10 = 1;
        while (tmp != 0)
        {
            tmp /= 10;
            pow10 *= 10;
        }
        /* concatenation: res = res * 10^d + term */
        res = res * pow10 + term;
    }

    return res / 6;
}
