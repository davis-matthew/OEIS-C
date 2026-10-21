/*
 * A051755  Consider problem of placing N queens on an n X n board so that
 *          each queen attacks precisely 2 others. Sequence gives maximal
 *          number of queens.
 * https://oeis.org/A051755
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 3 for n = 2, otherwise a(n) = 2*n - 2 for n >= 3
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)                     /* behavior below the offset is irrelevant */
        return (seq_t)0;
    if (n == 2)
        return (seq_t)3;
    /* n >= 3 */
    return (seq_t)n * ((seq_t)2) - (seq_t)2;
}
