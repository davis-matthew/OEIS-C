/*
 * A123663  Number of shared edges in a spiral of n unit squares.
 * https://oeis.org/A123663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n - ceil(2*sqrt(n))
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* defined output for n=0, not part of the offset */
        return 0;

    /* Compute floor(sqrt(n)) with a binary search */
    idx_t low = 1, high = n, s = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid <= n / mid) {          /* mid*mid <= n  */
            s = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Decide ceil(2*sqrt(n)) */
    idx_t sq = s * s;
    idx_t diff = n - sq;      /* n - s^2  */

    seq_t ceil2s;
    if (diff == 0) {                     /* perfect square */
        ceil2s = (seq_t)2 * (seq_t)s;
    } else if (diff <= s) {              /* 0 < f <= 0.5 */
        ceil2s = (seq_t)2 * (seq_t)s + 1;
    } else {                             /* f > 0.5 */
        ceil2s = (seq_t)2 * (seq_t)s + 2;
    }

    /* a(n) = 2*n - ceil(2*sqrt(n)) */
    seq_t result = (seq_t)2 * (seq_t)n - ceil2s;
    return result;
}
