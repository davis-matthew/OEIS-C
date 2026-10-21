/*
 * A203136  Indices of decagonal numbers that are also hexagonal.
 * https://oeis.org/A203136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3), a(1) = 1, a(2) = 20, a(3)
 *            = 667. This recurrence yields the indices of decagonal numbers
 *            that are also hexagonal (OEIS A203136)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* undefined for n < 1 */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)667;

    seq_t a_n_minus_3 = (seq_t)1;
    seq_t a_n_minus_2 = (seq_t)20;
    seq_t a_n_minus_1 = (seq_t)667;
    seq_t a_n = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        a_n = (seq_t)35 * a_n_minus_1
            - (seq_t)35 * a_n_minus_2
            + a_n_minus_3;

        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }

    return a_n;
}
