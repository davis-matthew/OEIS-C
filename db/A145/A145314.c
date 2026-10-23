/*
 * A145314  A 3 X 3 palindromic magic square read by rows.
 * https://oeis.org/A145314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, fini, full
 * Formula:   a(n) = p[(n-1) mod 9] with p = {6,1,8,7,5,3,2,9,4}
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the nth digit of the Lo Shu magic square read by rows,
   repeated every 9 elements.  idx_t n ≥ 1. */
seq_t a(idx_t n)
{
    /* Constant digit pattern of the Lo Shu magic square. */
    static const seq_t p[9] = {
        (seq_t)6, (seq_t)1, (seq_t)8,
        (seq_t)7, (seq_t)5, (seq_t)3,
        (seq_t)2, (seq_t)9, (seq_t)4
    };

    if (n == 0)          /* offset is 1 – values below 1 are undefined */
        return (seq_t)0;

    idx_t k = (n - 1) % 9;
    return p[k];
}
