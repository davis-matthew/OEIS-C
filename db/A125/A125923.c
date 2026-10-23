/*
 * A125923  Sprague-Grundy values for octal game .3337.
 * https://oeis.org/A125923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 if n%5==0, else n%5 for n>=20. For n≤19 the values are
 *            fixed: a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=0, a(6)=1, a(7)=2,
 *            a(8)=3, a(9)=4, a(10)=5, a(11)=1, a(12)=2, a(13)=3, a(14)=4,
 *            a(15)=5, a(16)=1, a(17)=2, a(18)=3, a(19)=0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values for 1 ≤ n ≤ 19 */
    static const seq_t init[19] = {
        1, 2, 3, 4, 0,
        1, 2, 3, 4, 5,
        1, 2, 3, 4, 5,
        1, 2, 3, 0
    };

    if (n <= 19) {
        /* n is at least 1 and at most 19; no overflow in subtraction */
        return init[(size_t)(n - 1)];
    }

    /* Periodic part for n ≥ 20:
       5,1,2,3,4 repeating. */
    idx_t r = n % 5;
    return (r == 0 ? (seq_t)5 : (seq_t)r);
}
