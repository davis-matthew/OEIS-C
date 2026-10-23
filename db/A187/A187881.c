/*
 * A187881  Triangle read by rows: the n-th column consists of n n's followed
 *          by 0's.
 * https://oeis.org/A187881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (i < 2*j) ? j : 0, where * i = smallest integer with
 *            i*(i+1)/2 >= n * j = n - i*(i-1)/2 * This implements the
 *            triangle read by rows: each column j contains * j consecutive
 *            j's followed by zeros
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 by definition; handle the impossible n==0 case safely. */
    if (n == 0) return (seq_t)0;

    /* Binary search for i: 1 <= i <= n, minimal with i(i+1)/2 >= n. */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 tri_mid = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri_mid >= n) hi = mid;
        else lo = mid + 1;
    }
    idx_t i = lo;

    /* Compute j = n - T(i-1), where T(k) = k(k+1)/2. */
    unsigned __int128 tri_prev = (unsigned __int128)(i - 1) * i / 2;
    idx_t j = n - (idx_t)tri_prev;

    /* Return column value: j if i < 2*j, otherwise 0. */
    return ((unsigned __int128)i < (unsigned __int128)2 * j) ? (seq_t)j : (seq_t)0;
}
