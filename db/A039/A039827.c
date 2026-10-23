/*
 * A039827  Number of different coefficient values in expansion of Product
 *          (1+q^i+q^(2i)), i=1 to n.
 * https://oeis.org/A039827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 - 1 for n >= 9. For 1 ≤ n ≤ 8 the values are:
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=6, a(5)=11, a(6)=19, a(7)=26,
 *            a(8)=34
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n<1 – return 0 */
        return (seq_t)0;

    /* Special cases for n ≤ 8 */
    if (n <= 8) {
        static const seq_t vals[8] = {1, 2, 3, 6, 11, 19, 26, 34};
        return vals[n - 1];
    }

    /* General formula for n ≥ 9 */
    seq_t nn = (seq_t)n;
    return nn * (nn + (seq_t)1) / (seq_t)2 - (seq_t)1;
}
