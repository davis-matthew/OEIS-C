/*
 * A212850  Number of n X 3 arrays with rows being permutations of 0..2 and no
 *          column j greater than column j-1 in all rows.
 * https://oeis.org/A212850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^n - 2*3^n + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* a(0) is undefined; return 0 for safety */

    seq_t pow6 = (seq_t)1;
    seq_t pow3 = (seq_t)1;

    /* compute 6^n and 3^n using a simple loop – works for any seq_t size */
    for (idx_t i = 0; i < n; ++i) {
        pow6 *= (seq_t)6;
        pow3 *= (seq_t)3;
    }

    /* a(n) = 6^n - 2*3^n + 1 */
    return pow6 - (seq_t)2 * pow3 + (seq_t)1;
}
