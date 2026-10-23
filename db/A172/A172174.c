/*
 * A172174  a(n) = 90*a(n-1) + 1, with a(1) = 1.
 * https://oeis.org/A172174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 90*a(n-1) + 1, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)      /* Undefined for offset 1, return 0 for safety */
        return 0;

    seq_t val = 1;    /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
        val = val * (seq_t)90 + (seq_t)1;

    return val;
}
