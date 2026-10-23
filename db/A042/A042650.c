/*
 * A042650  Numerators of continued fraction convergents to sqrt(855).
 * https://oeis.org/A042650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6082*a(n-4) - a(n-8)
   a(0)=29, a(1)=117, a(2)=731, a(3)=3041,
   a(4)=177109, a(5)=711477, a(6)=4445971, a(7)=18495361 */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        29, 117, 731, 3041,
        177109, 711477, 4445971, 18495361
    };

    seq_t buf[8];
    size_t k;
    for (k = 0; k < 8; ++k) {
        buf[k] = init[k];
    }

    if (n < 8) {
        return buf[n];
    }

    idx_t i;
    for (i = 8; i <= n; ++i) {
        size_t idx  = (size_t)i & 7;
        size_t idx4 = ((size_t)i - 4) & 7;
        size_t idx8 = ((size_t)i - 8) & 7;
        buf[idx] = (seq_t)6082 * buf[idx4] - buf[idx8];
    }

    return buf[(size_t)n & 7];
}
