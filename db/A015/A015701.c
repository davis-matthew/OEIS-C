/*
 * A015701  From iteration of the Galton-Watson branching process.
 * https://oeis.org/A015701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 3 a(n) = a(n-1)^2 + 2^(2^(n+1) - 2) for n ≥ 1 (OEIS
 *            A015701)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;

    seq_t cur  = (seq_t)3;      /* a(0) */
    seq_t add  = (seq_t)4;      /* add_1 = 2^(2^(1+1)-2) = 2^2 = 4 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* a(i) = a(i-1)^2 + add_i */
        cur = cur * cur;
        cur = cur + add;

        if (i == n)
            break;           /* no next addition needed */

        /* compute next addition: add_{i+1} = add_i^2 * 4   */
        add = add * add * (seq_t)4;
    }

    return cur;
}
