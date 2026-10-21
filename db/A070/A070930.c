/*
 * A070930  Smallest integer >= 0 of the form x^3 - n^4.
 * https://oeis.org/A070930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(n^(4/3))^3 - n^4 Equivalent: find the minimal
 *            integer k such that k^3 >= n^4 and return k^3 - n^4 (the result
 *            is zero when n is a perfect cube)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n^4 */
    seq_t nn = (seq_t)n;
    seq_t nsq = nn * nn;      /* n^2   */
    seq_t n4  = nsq * nsq;    /* n^4   */

    /* integer cube root floor of n4 via exponential bound + binary search */
    seq_t low  = 0;
    seq_t high = 1;
    /* enlarge high until high^3 >= n4 */
    while ( (high * high * high) < n4 ) {
        high = high * 2;
    }
    /* binary search for floor cube root */
    while (low < high) {
        seq_t mid = (low + high + 1) / 2;   /* +1 to bias toward upper half */
        seq_t mid_cub = mid * mid * mid;
        if (mid_cub <= n4) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    seq_t root = low;
    seq_t root_cub = root * root * root;

    /* if n4 is already a perfect cube, result is zero */
    if (root_cub == n4) {
        return (seq_t)0;
    }

    /* otherwise use the next integer cube */
    seq_t next = root + 1;
    seq_t next_cub = next * next * next;

    return next_cub - n4;
}
