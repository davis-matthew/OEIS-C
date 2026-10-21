/*
 * A063724  Consider problem of placing N queens on an n X n board so that
 *          each queen attacks precisely 4 others. Sequence gives maximal
 *          number of queens.
 * https://oeis.org/A063724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0, n=1,2,3; a(4)=8; a(5)=11; a(n)=3*n-3 for n>=6
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    if (n == 1 || n == 2 || n == 3)
        return (seq_t)0;

    if (n == 4)
        return (seq_t)8;

    if (n == 5)
        return (seq_t)11;

    /* For n >= 6: a(n) = 3*n - 3 */
    return (seq_t)(3ULL * n - 3);
}
