/*
 * A053700  a(n) = 111111 in base n.
 * https://oeis.org/A053700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = n^5 + n^4 + n^3 + n^2 + n + 1
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t power = 1;                /* n^0 */
    seq_t rn = (seq_t)n;           /* convert once to seq_t */

    for (int i = 0; i < 6; ++i) {   /* i = 0..5  */
        res += power;              /* add n^i */
        power *= rn;               /* update to n^(i+1) */
    }
    return res;
}
