/*
 * A217111  Number of pandigital numbers <= 10^n.
 * https://oeis.org/A217111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 3265920 * Σ_{j=1..n} S2(j,10), where S2(j,k) is the
 *            Stirling number of the second kind. The recurrence for S2 with
 *            fixed k is S2(0,0)=1, S2(j,0)=0 (j>0),
 *            S2(j,k)=k*S2(j-1,k)+S2(j-1,k-1)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 11, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* The largest k we need is 10. */
    seq_t prev[11] = {0}, cur[11] = {0};
    prev[0] = (seq_t)1;               /* S2(0,0) = 1 */
    seq_t sum = (seq_t)0;             /* Σ S2(j,10) for j ≤ current */
    for (idx_t j = 1; j <= n; ++j) {
        cur[0] = (seq_t)0;
        for (int k = 1; k <= 10; ++k) {
            cur[k] = (seq_t)k * prev[k] + prev[k - 1];
        }
        sum += cur[10];
        /* copy cur to prev for next iteration */
        for (int k = 0; k <= 10; ++k) {
            prev[k] = cur[k];
        }
    }
    return (seq_t)3265920 * sum;
}
