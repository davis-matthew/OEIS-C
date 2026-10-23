/*
 * A137693  Numbers n such that 3n^2-n = 6k^2-2k for some integer k>0.
 * https://oeis.org/A137693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1155*a(n-1)-1155*a(n-2)+a(n-3), a(1)=7, a(2)=7887,
 *            a(3)=9101399
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms directly */
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)7887;
    if (n == 3) return (seq_t)9101399;

    seq_t a1 = (seq_t)7;        /* a(1) */
    seq_t a2 = (seq_t)7887;     /* a(2) */
    seq_t a3 = (seq_t)9101399;  /* a(3) */
    seq_t a4 = a3;              /* placeholder for a(n) */

    const seq_t coef = (seq_t)1155;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(n) = 1155*a(n-1) - 1155*a(n-2) + a(n-3) */
        a4 = a3 * coef - a2 * coef + a1;

        /* shift values for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }

    return a4;
}
