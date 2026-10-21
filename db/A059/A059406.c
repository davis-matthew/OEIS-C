/*
 * A059406  a(n) = a(n-1)*(3a(n-1) + 1)/2 with a(1) = 1.
 * https://oeis.org/A059406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*(3*a(n-1)+1)/2 (a(1)=1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 for safety */
    if (n == 0) return 0;

    seq_t cur = 1;                     /* a(1) = 1 */

    for (idx_t i = 2; i <= n; ++i) {
        /* compute cur = cur*(3*cur+1)/2 */
        seq_t temp = 3 * cur;          /* 3*cur */
        temp += 1;                    /* 3*cur + 1 */
        cur = (cur * temp) / 2;       /* (cur*temp)/2 */
    }
    return cur;
}
