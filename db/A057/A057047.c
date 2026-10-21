/*
 * A057047  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057047(n)=j(2^n).
 * https://oeis.org/A057047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j(2^n) where j(k) = d(d+1)/2 + 1 - k and * d =
 *            ceil((sqrt(8k+1)-1)/2). We compute d by a binary search * on d
 *            such that d(d+1)/2 ≥ k. All operations are performed on * the
 *            generic type seq_t. The function works for any seq_t * (uint8_t
 *            .. arbitrary‑precision class) and for any n ≥ 0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = 2^n as a seq_t. */
    seq_t k = 1;
    for (idx_t i = 0; i < n; ++i)
        k <<= 1;                 /* multiply by 2 */

    /* Binary search for the smallest d with d(d+1)/2 >= k. */
    seq_t low  = 0;
    seq_t high = k;              /* high is always a valid upper bound */

    while ((low + 1) < high) {
        seq_t mid = (low + high) / 2;
        seq_t tri = mid * (mid + 1) / 2;
        if (tri >= k)
            high = mid;
        else
            low = mid;
    }
    seq_t d = high;

    /* j = d(d+1)/2 + 1 - k. */
    seq_t j = d * (d + 1) / 2 + 1 - k;
    return j;
}
