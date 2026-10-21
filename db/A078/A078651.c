/*
 * A078651  Number of increasing geometric-progression subsequences of
 *          [1,...,n] with integral successive-term ratio and length >= 1.
 * https://oeis.org/A078651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + Sum_{r>1, j>0} floor(n/r^j)
 * Range:     u8 n <= 53, u16 n <= 6600, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /*sequence is defined for n ≥ 1, but guard anyway*/
    seq_t sum = (seq_t)n;           /*single-term subsequences*/

    for (idx_t r = 2; r <= n; ++r) {
        idx_t pow = r;
        /* add floor(n / r^j) for all j ≥ 1 while r^j ≤ n */
        while (pow <= n) {
            sum += (seq_t)(n / pow);
            /* prevent overflow in pow *= r */
            if (pow > n / r) break;
            pow *= r;
        }
    }
    return sum;
}
