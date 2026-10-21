/*
 * A073849  Cumulative sum of initial digits of (n base 3).
 * https://oeis.org/A073849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = Sum_{i=0..n} A000030(A007089(i))
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t p = 1;

    while (p <= n) {
        /* Leading digit 1 in the block [p, 2p-1] (or up to n). */
        idx_t start1 = p;
        idx_t end1 = (p > n / 2) ? n : 2 * p - 1;
        if (start1 <= end1) {
            idx_t cnt1 = end1 - start1 + 1;
            sum += (seq_t)cnt1;          /* digit 1 * count */
        }

        /* Leading digit 2 in the block [2p, 3p-1] (or up to n). */
        idx_t start2 = (p <= n / 2) ? 2 * p : n + 1;  /* n+1 > n means no contribution */
        if (start2 <= n) {
            idx_t end2 = (p > n / 3) ? n : 3 * p - 1;
            if (start2 <= end2) {
                idx_t cnt2 = end2 - start2 + 1;
                sum += (seq_t)cnt2 * 2;  /* digit 2 * count */
            }
        }

        /* Advance to the next power of three. */
        if (p > n / 3)
            break;    /* next power would exceed n */
        p *= 3;
    }
    return sum;
}
