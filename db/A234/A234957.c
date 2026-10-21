/*
 * A234957  Highest power of 4 dividing n.
 * https://oeis.org/A234957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 4^(floor(v2(n)/2))
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(4n) = 4 * a(n), a(4n + k) = 1 for k = 1, 2, 3 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)  /* for safety; offset is 1 in OEIS */
        return (seq_t)1;

    /* Count trailing zeros in binary representation of n */
    idx_t tz = 0;
    idx_t m  = n;
    while ((m & (idx_t)1) == 0) {
        tz++;
        m >>= 1;
    }

    /* e = floor(v2(n)/2) */
    idx_t e = tz >> 1;          /* tz / 2 */

    /* a(n) = 4^e = 2^(2e) = 1 << (2e) */
    return ((seq_t)1) << (2 * e);
}
