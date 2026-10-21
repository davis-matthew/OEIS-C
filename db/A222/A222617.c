/*
 * A222617  Number of integers k < n whose Collatz (3x+1) trajectory contains
 *          n.
 * https://oeis.org/A222617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 849, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = #{k < n | n appears in the Collatz trajectory of k} */

seq_t a(idx_t n)
{
    /* count of integers k<n whose Collatz sequence reaches n */
    seq_t cnt = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t x = k;
        while (x > 1) {
            if (x == n) {
                ++cnt;          /* k < n, so count this k */
                break;          /* stop when n is found */
            }
            if (x & 1) {
                x = 3 * x + 1;  /* odd case */
            } else {
                x >>= 1;        /* even case */
            }
        }
    }
    return cnt;
}
