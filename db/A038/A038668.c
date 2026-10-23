/*
 * A038668  a(n)=[ n/3 ] + [ n/4 ] + [ n/7 ] + [ n/11 ] + [ n/18 ] + [ n/29 ]
 *          + [ n/47 ] + [ n/76 ] + [ n/123 ] + [ n/199 ]... (using Lucas
 *          numbers A000204).
 * https://oeis.org/A038668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⎣n/3⎦ + ⎣n/4⎦ + ⎣n/7⎦ + ⎣n/11⎦ + ⎣n/18⎦ + ⎣n/29⎦ + …,
 *            where the denominators are Lucas numbers L_k (A000204) starting
 *            with 3 (i.e. k=2 onward). The sum runs while L_k ≤ n
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset n ≥ 1; for n < 3 the sum is empty. */
    seq_t N = (seq_t)n;   /* cast once for all divisions */
    seq_t sum = 0;

    /* Lucas numbers: L_0 = 2, L_1 = 1, L_{k+1} = L_k + L_{k-1} */
    seq_t l_prev = 2;  /* L_0 */
    seq_t l_cur  = 1;  /* L_1 */

    while (1) {
        seq_t l_next = l_prev + l_cur;   /* L_{k+1} */
        if (l_next > N) break;
        sum += N / l_next;               /* floor division */
        l_prev = l_cur;
        l_cur  = l_next;
    }
    return sum;
}
