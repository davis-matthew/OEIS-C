/*
 * A145910  a(n) = (1 + 3*n)*(4 + 3*n)/2.
 * https://oeis.org/A145910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 3*n)*(4 + 3*n)/2
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A145910 */
seq_t a(idx_t n)
{
    /* cast index to seq_t first */
    seq_t n1 = (seq_t)n;
    seq_t three_n = n1 * 3;
    seq_t a_val = three_n + 1;   /* 3*n + 1 */
    seq_t b_val = three_n + 4;   /* 3*n + 4 */

    /* one of a_val, b_val is always even, divide it by 2 to keep the
       intermediate product small and to avoid overflow if seq_t is
       narrower. */
    if ((a_val & 1) == 0)
        a_val /= 2;
    else
        b_val /= 2;

    return a_val * b_val;
}
