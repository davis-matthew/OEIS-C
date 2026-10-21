/*
 * A081188  6th binomial transform of (1,0,1,0,1,.....), A059841.
 * https://oeis.org/A081188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 35*a(n-2), a(0)=1, a(1)=6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)6;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t temp = a1 * (seq_t)12;
        temp -= a0 * (seq_t)35;
        a0 = a1;
        a1 = temp;
    }

    return a1;
}
