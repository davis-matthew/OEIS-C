/*
 * A043490  Numbers having two 0's in base 10.
 * https://oeis.org/A043490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3056, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having two 0's in base 10 (OEIS A043490) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, but guard against 0 */

    idx_t count = 0;                /* how many qualifying numbers found so far */

    for (idx_t num = 1;; ++num) {   /* iterate over all positive integers */
        idx_t t = num;
        int zeros = 0;

        while (t != 0) {
            if ((t % 10) == 0) {
                ++zeros;
                if (zeros > 2) break;   /* early exit if too many zeros */
            }
            t /= 10;
        }

        if (zeros == 2) {            /* this number has exactly two zeros */
            ++count;
            if (count == n) {
                return (seq_t)num;    /* cast to the arbitrary‑precision type */
            }
        }
    }
}
