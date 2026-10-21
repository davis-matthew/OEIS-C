/*
 * A080800  Similar to A080799 but count only division steps.
 * https://oeis.org/A080800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(log2(n + 1)) + 3 (OEIS A080800 – the number of
 *            division steps in the Euclidean algorithm for two successive
 *            triangular numbers)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Since n starts from 1, a(1) = floor(log2(2)) + 3 = 4, matching the
   OEIS data.  The values grow slowly and easily fit in seq_t.  */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* Handle the special case of n == UINT64_MAX to avoid overflow when
       converting n + 1 into a 64‑bit integer.  For this enormous n the
       value of a(n) is 64 + 3 = 67, which comfortably fits inside seq_t. */
    if (n == UINT64_MAX) {
        return (seq_t)(64 + 3);
    }

    /* n + 1 is always non‑zero here, so we can safely compute its
       floor(log2) using the built‑in count‑leading‑zeros operation. */
    uint64_t m = n + 1;               /* m > 0 */
    unsigned bits = 63 - __builtin_clzll(m);

    /* The formula: floor(log2(n + 1)) + 3 */
    return (seq_t)(bits + 3);
}
