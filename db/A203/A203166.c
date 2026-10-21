/*
 * A203166  (n-1)-st elementary symmetric function of the first n terms of
 *          (1,2,3,4,5,1,2,3,4,5,...)=A010884.
 * https://oeis.org/A203166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (∏_{i=1}^{n} ((i-1) mod 5 + 1)) *
 *               (∑_{i=1}^{n} 1 / ((i-1) mod 5 + 1))
 *
 *  The sequence A203166 is the (n-1)-st elementary symmetric function of the
 *  first n terms of the repeating sequence 1,2,3,4,5.
 *  This implementation uses the algebraic simplification:
 *    a(n) = P(n) * S(n)
 *  where P(n) = 120^{⌊n/5⌋} * ∏_{i=1}^{n%5} i
 *        S(n) = (⌊n/5⌋ * 137 + R) / 60
 *  with R = {0, 60, 90, 110, 125} for n%5 = {0,1,2,3,4}.
 *
 *  The function works for any idx_t n ≥ 1 and returns the exact value
 *  in the seq_t type.
 */

static const seq_t prod_small[5] = { 1, 1, 2, 6, 24 };   /* product of 1..r   */
static const seq_t sum_r_int[5] = { 0, 60, 90, 110, 125 };/* partial sum 137/60 * r */

seq_t a(idx_t n)
{
    idx_t q = n / 5;          /* number of full 1..5 blocks      */
    int r   = (int)(n % 5);   /* remaining elements  (0..4)      */

    /* Handle the first 4 terms directly (q == 0) */
    if (q == 0) {
        seq_t P = prod_small[r];
        seq_t N = sum_r_int[r];
        seq_t res = P * N;
        res /= 60;                 /* division is exact */
        return res;
    }

    /* For q >= 1 we can divide by 60 early:
     *    a(n) = (2 * 120^(q-1) * prod_small[r]) * (q*137 + sum_r_int[r])
     */
    seq_t pow = 1;
    for (idx_t i = 1; i < q; ++i) {
        pow *= (seq_t)120;          /* compute 120^(q-1)       */
    }

    seq_t base = pow * 2;           /* 2 * 120^(q-1)           */
    seq_t P_div = base * prod_small[r];
    seq_t N = (seq_t)q * 137 + sum_r_int[r];
    seq_t res = P_div * N;          /* exact, division already factored out */

    return res;
}
