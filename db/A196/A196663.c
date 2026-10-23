/*
 * A196663  Expansion of g.f. (1-4*x)/(1-13*x).
 * https://oeis.org/A196663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 9, a(n) = 13*a(n-1) (n >= 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0) {
        return 1;
    }

    /* For n >= 1, start from 9 and multiply by 13 for each subsequent step */
    seq_t result = 9;
    for (idx_t i = 1; i < n; ++i) {
        result *= 13;
    }

    return result;
}
