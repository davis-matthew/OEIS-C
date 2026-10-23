/*
 * A031955  Numbers with exactly two distinct base-10 digits.
 * https://oeis.org/A031955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 1606, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A031955: Numbers with exactly two distinct base-10 digits.
   The algorithm scans integers sequentially from 10 upwards, counts those
   with exactly two distinct decimal digits, and returns the n-th such number.
   Complexity: O(n) with at most a few multiples of n iterations. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* out of range offset */
    idx_t count = 0;
    uint64_t x = 10;                             /* start search at 10 */
    for (;; ++x) {
        uint64_t t = x;
        int  seen = 0;                           /* bit mask of seen digits */
        int  dcnt = 0;                           /* distinct digit count */
        while (t > 0) {
            int d = (int)(t % 10);
            t /= 10;
            int bit = 1 << d;
            if ((seen & bit) == 0) {            /* new digit seen */
                seen |= bit;
                ++dcnt;
                if (dcnt > 2) break;            /* early exit */
            }
        }
        if (dcnt == 2) {
            ++count;
            if (count == n)
                return (seq_t)x;                 /* found the n‑th term */
        }
    }
}
