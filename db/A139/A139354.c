/*
 * A139354  Let the binary expansion of n be n = Sum_{k} 2^{r_k}, let e(n) be
 *          the number of r_k's that are even, o(n) the number that are odd;
 *          sequence gives min{e(n), o(n)}.
 * https://oeis.org/A139354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{e(n), o(n)}, where e(n) = number of 1‑bits at even
 *            bit positions in n (positions 0, 2, 4, …) and o(n) = number of
 *            1‑bits at odd bit positions (positions 1, 3, 5, …)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t even = 0;          /* count of 1‑bits at even positions   */
    seq_t odd  = 0;          /* count of 1‑bits at odd positions    */
    idx_t pos  = 0;          /* current bit position (0 = LSB)      */
    idx_t m = n;             /* working copy of the input           */

    while (m) {
        if (m & 1ULL) {      /* if the current bit is set           */
            if ((pos & 1ULL) == 0)
                even++;
            else
                odd++;
        }
        m >>= 1;             /* move to the next bit                */
        pos++;
    }

    return (even < odd) ? even : odd;
}
