/*
 * A191254  Fixed point of the morphism 0 -> 01, 1 -> 02, 2 -> 01.
 * https://oeis.org/A191254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is odd, a(n) = 1 if n is even and v2(n) is odd,
 *            a(n) = 2 if n is even and v2(n) is even, where v2(n) is the
 *            2‑adic valuation of n (number of trailing zeros in binary)
 * Range:     u8 n <= 9744, u16 n <= 9744, u32 n <= 9744, u64 n <= 9744,
 *            u128 n <= 9744, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned int trailing_zeros(idx_t n) {
    unsigned int r = 0;
    /* n > 0 when this function is used in a(idx_t) except for the unlikely n==0 case */
    while ((n & 1) == 0) {
        r++;
        n >>= 1;
    }
    return r;
}

seq_t a(idx_t n) {
    /* Off‑by‑one handling: the sequence is defined for n >= 1.  If n == 0 we
       return 0 (not used by the test harness). */
    if (n == 0) return (seq_t)0;

    if ((n & 1) == 1) {            /* n is odd */
        return (seq_t)0;
    } else {                       /* n is even */
        unsigned int v = trailing_zeros(n);
        return (seq_t)((v & 1) ? 1 : 2);
    }
}
