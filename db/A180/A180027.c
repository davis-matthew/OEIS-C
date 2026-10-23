/*
 * A180027  Partial sums of A100706.
 * https://oeis.org/A180027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (10^(2*k+1)-1)/9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t term = 1; /* 2*0+1 = 1 digit of '1' */
    for (idx_t k = 0; k <= n; ++k) {
        sum += term;
        if (k == n) break;         /* last iteration: no next term needed */
        term = term * 100 + 11;     /* build the next 2*k+3 digit string of '1's */
    }
    return sum;
}
