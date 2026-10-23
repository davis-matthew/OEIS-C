/*
 * A045757  10-factorial numbers.
 * https://oeis.org/A045757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=0}^{n-1} (10*k + 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {            /* not part of the sequence, but safe guard */
        return 0;
    }
    seq_t res = 1;
    /* Multiply successive factors (10*i + 1) for i = 1 .. n-1 */
    for (idx_t i = 1; i < n; ++i) {
        res *= ((seq_t)10 * i + 1);
    }
    return res;
}
