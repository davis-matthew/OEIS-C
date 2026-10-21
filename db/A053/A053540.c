/*
 * A053540  a(n) = n*9^(n-1).
 * https://oeis.org/A053540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * 9^(n-1) (offset 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is defined only for n >= 1 */
        return (seq_t)0;
    seq_t pow = 1;          /* 9^(n-1) */
    idx_t exp = n - 1;

    for (idx_t i = 0; i < exp; ++i)
        pow *= (seq_t)9;

    return (seq_t)n * pow;
}
