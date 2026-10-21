/*
 * A228356  The triangle associated with the family of polynomials W_n(x).
 * https://oeis.org/A228356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 22, i16 n <= 47, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A228356: a(n) is the value W_i(j) where i+j = s and
 * s is the unique integer with s(s+1)/2 ≤ n < (s+1)(s+2)/2,
 * and j = n - s(s+1)/2, i = s - j.  The polynomials W_k(x) are
 * defined by:
 *     W_0(x) = 1
 *     W_1(x) = 2x + 1
 *     W_{k+1}(x) = 2x * W_k(x) - W_{k-1}(x)
 *
 * The implementation finds s by a binary search on the triangular
 * numbers and then evaluates W_i(j) iteratively.
 */

seq_t a(idx_t n)
{
    /* find s: largest integer such that s*(s+1)/2 <= n */
    idx_t low = 0;
    idx_t high = 1;
    /* grow high until triangular(high) > n */
    while ( ((__int128)high * (high + 1) / 2) <= n )
        high <<= 1U; 
    /* binary search */
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        if ( ((__int128)mid * (mid + 1) / 2) <= n )
            low = mid;
        else
            high = mid - 1;
    }
    idx_t s = low;
    /* position within the block of size s+1 */
    idx_t pos = n - (__int128)s * (s + 1) / 2;
    idx_t i = s - pos;   /* first index (row)  */
    idx_t j = pos;       /* second index (col) */

    /* evaluate W_i(j) by recurrence */
    if (i == 0)
        return (seq_t)1;
    /* W_1(j) */
    seq_t w_prev = (seq_t)1;                /* W_0(j) */
    seq_t w_cur = (seq_t)2 * (seq_t)j + 1;   /* W_1(j) */
    if (i == 1)
        return w_cur;

    for (idx_t k = 2; k <= i; ++k) {
        seq_t w_next = (seq_t)2 * (seq_t)j * w_cur - w_prev;
        w_prev = w_cur;
        w_cur = w_next;
    }
    return w_cur;
}
