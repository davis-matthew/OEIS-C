/*
 * A127250  Sequence consisting of 1,3 or 5 with 3's occurring at the odious
 *          indices given by A091855 and 5's occurring at twice these odious
 *          indices.
 * https://oeis.org/A127250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   * a(n) = 1, unless n is odious (has odd popcount). * If n is
 *            odious and the exponent of 2 in n is odd, a(n) = 5; * otherwise
 *            a(n) = 3. * (n is odious ⇔ popcount(n) is odd.)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static inline bool popcount_odd(idx_t x)
{
    bool odd = false;
    while (x) {
        odd = !odd;
        x &= x - 1;                 /* drop lowest set bit */
    }
    return odd;
}

/* Return the sequence value a(n). */
seq_t a(idx_t n)
{
    if (!popcount_odd(n))
        return (seq_t)1;          /* not odious */

    /* Compute parity of v₂(n) (number of trailing zeros). */
    bool tz_parity = false;       /* false = even, true = odd */
    idx_t m = n;
    while ((m & 1ULL) == 0) {     /* n > 0 if we reach this code */
        tz_parity = !tz_parity;
        m >>= 1;
    }

    return tz_parity ? (seq_t)5 : (seq_t)3;
}
