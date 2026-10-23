/*
 * A135454  Number of sums payable using exactly n banknotes of denominations
 *          1, 5, 10, 20, 50, 100.
 * https://oeis.org/A135454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 99*n - 330 for n >= 9; a(0..8) are special cases
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special values for n = 0 … 8. */
    static const seq_t tbl[9] = {
        (seq_t)1,  /* a(0) */
        (seq_t)6,  /* a(1) */
        (seq_t)21, /* a(2) */
        (seq_t)52, /* a(3) */
        (seq_t)103,/* a(4) */
        (seq_t)174,/* a(5) */
        (seq_t)262,/* a(6) */
        (seq_t)360,/* a(7) */
        (seq_t)461 /* a(8) */
    };

    if (n < 9)
        return tbl[n];

    seq_t nn = (seq_t)n;
    return 99 * nn - 330;
}
