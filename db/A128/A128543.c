/*
 * A128543  a(n) = floor(2^(n-2)*3*n).
 * https://oeis.org/A128543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n*2^(n-2)) for n ≥ 2, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, behaviour below offset is irrelevant */
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */
    if (n == 2) return (seq_t)6;   /* a(2) = 6 */

    seq_t a_prev = (seq_t)6;       /* value of a(2) */
    for (idx_t cur = 3; cur <= n; ++cur) {
        /* a(cur) = a(cur-1) / (cur-1) * 2 * cur   (exact division) */
        seq_t temp = a_prev / (cur - 1);   /* exact division by (cur-1) */
        temp *= (seq_t)2;                 /* multiply by 2 */
        temp *= (seq_t)cur;               /* multiply by cur */
        a_prev = temp;                    /* prepare for next iteration */
    }
    return a_prev;                       /* a(n) */
}
