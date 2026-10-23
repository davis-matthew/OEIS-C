/*
 * A048158  Triangular array T read by rows: T(n,k) = n mod k, for
 *          k=1,2,...,n, n=1,2,...
 * https://oeis.org/A048158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (row of the triangular array for index n) mod (column
 *            index) This implementation finds the row containing the nth term
 *            via binary search on the triangular number N*(N+1)/2, then
 *            computes k = n - N*(N-1)/2 and returns N % k
 * Range:     u8 n <= 45, u16 n <= 4149, u32 n <= 9977, u64 n <= 9977,
 *            u128 n <= 9977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* offset starts at 1, but guard anyway */

    idx_t low = 1;
    idx_t high = n;                    /* n is always >= row number */

    /* binary search for the smallest row such that row*(row+1)/2 >= n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t mid_seq = (seq_t)mid;
        seq_t tot = mid_seq * (mid_seq + 1) / 2;
        if (tot >= (seq_t)n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    seq_t row = (seq_t)low;            /* the row containing the nth term */
    seq_t prev = row * (row - 1) / 2;  /* total elements before this row   */
    seq_t k = (seq_t)n - prev;         /* column index within the row      */

    return row % k;                    /* row mod column gives the term    */
}
