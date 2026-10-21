/*
 * A051756  Consider the problem of placing N queens on an n X n board so that
 *          each queen attacks precisely 3 others. Sequence gives maximal
 *          number of queens.
 * https://oeis.org/A051756
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 2 * floor((6*n - 2)/5) (conjectured exact for all n ≥ 2)
 * Range:     u8 n <= 42, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (6*n - 2) as a seq_t value */
    seq_t t = (seq_t)6 * (seq_t)n - (seq_t)2;
    /* Integer division truncates toward zero; for n ≥ 2 floor is exact */
    seq_t q = t / (seq_t)5;
    /* Multiply by 2 to get the announced value */
    return q * (seq_t)2;
}
