/*
 * A059838  Number of permutations in the symmetric group S_n that have even
 *          order.
 * https://oeis.org/A059838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(2n) = (2n-1)! + (2n-1)a(2n-1); a(2n+1) = (2n+1)a(2n); a(0)=0,
 *            a(1)=0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;   /* a(0) */
    seq_t fact   = 1;   /* (0)! */

    for (idx_t k = 1; k <= n; ++k) {
        if ((k & 1) == 0) {          /* k even: a(k) = (k-1)! + (k-1)a(k-1) */
            result = fact + ((seq_t)(k - 1) * result);
        } else {                     /* k odd:  a(k) = k * a(k-1) */
            result = (seq_t)k * result;
        }
        fact = fact * k;             /* update factorial: (k)! for next round */
    }

    return result;
}
