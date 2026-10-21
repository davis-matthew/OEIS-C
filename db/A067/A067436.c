/*
 * A067436  a(n) = sum of all the remainders when n-th even number is divided
 *          by even numbers < 2n.
 * https://oeis.org/A067436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * sum_{k=1}^{n-1} (n mod k) = 2 * [ n*(n-1) -
 *            sum_{k=1}^{n-1} k * floor(n/k) ]
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    const idx_t limit = n - 1;          /* highest divisor index (k=n-1)          */
    const seq_t n_seq = (seq_t)n;
    const seq_t n_minus1_seq = (seq_t)limit;
    const seq_t totalsum = n_seq * n_minus1_seq;   /* n*(n-1)                         */

    seq_t sum_k_floor = 0;              /* sum_{k} k * floor(n/k)           */
    idx_t i = 1;

    while (i <= limit) {
        /* floor(n / i) is constant over k in [i, R] */
        idx_t q = n / i;
        idx_t R = n / q;
        if (R > limit)
            R = limit;

        /* sum of k from i to R: (i+R)*(R-i+1)/2 */
        seq_t i_seq = (seq_t)i;
        seq_t R_seq = (seq_t)R;
        seq_t cnt   = (seq_t)(R - i + 1);
        seq_t sum_k = (i_seq + R_seq) * cnt / 2;

        /* add contribution q * sum_k */
        sum_k_floor += (seq_t)q * sum_k;

        i = R + 1;
    }

    const seq_t rem_sum = totalsum - sum_k_floor;   /* sum of remainders */
    return rem_sum * 2;                             /* final a(n) */
}
