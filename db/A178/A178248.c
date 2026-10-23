/*
 * A178248  a(n) = 12^n + 1.
 * https://oeis.org/A178248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;          /* start with 12^0 = 1 */
    for (idx_t i = 0; i < n; ++i)
        val *= (seq_t)12;          /* multiply by 12 each step */

    return val + (seq_t)1;        /* add 1 to get 12^n + 1 */
}
