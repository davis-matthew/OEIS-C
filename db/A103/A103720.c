/*
 * A103720  Column m=3 sequence (unsigned) of triangle A103718(n,m), n>=0,
 *          without leading zeros.
 * https://oeis.org/A103720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |S1(n+4,4)| + |S1(n+4,5)|, unsigned Stirling numbers of
 *            first kind
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static idx_t max_sz = 1;
    static seq_t *C[6] = {0};   /* C[k][i] = |s(i,k)| */

    idx_t N = n + 5;            /* need values up to n+5 (for k=5) */

    /* first-time allocation: base cases C[0][0] = 1, others 0 */
    if (max_sz == 1 && C[0] == NULL) {
        for (idx_t k = 0; k <= 5; ++k) {
            C[k] = (seq_t*)malloc(sizeof(seq_t));
            C[k][0] = (k == 0 ? (seq_t)1 : (seq_t)0);
        }
    }

    /* expand tables if necessary */
    if (max_sz <= N) {
        idx_t new_sz = N + 1;
        for (idx_t k = 0; k <= 5; ++k) {
            seq_t *tmp = (seq_t*)realloc(C[k], new_sz * sizeof(seq_t));
            /* initialize new entries to 0 */
            for (idx_t i = max_sz; i < new_sz; ++i) tmp[i] = (seq_t)0;
            C[k] = tmp;
        }
        /* compute new rows using recurrence for unsigned Stirling numbers */
        for (idx_t i = max_sz; i <= N; ++i) {
            for (idx_t k = 1; k <= 5 && k <= i; ++k) {
                seq_t part1 = C[k-1][i-1];
                seq_t part2 = C[k][i-1];
                seq_t mult   = (seq_t)(i-1) * part2;
                C[k][i] = part1 + mult;
            }
        }
        max_sz = new_sz;
    }

    /* a(n) = |S1(n+4,4)| + |S1(n+4,5)| */
    return C[4][n+4] + C[5][n+4];
}
