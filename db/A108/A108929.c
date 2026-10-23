/*
 * A108929  Expansion of (1-x^4-2*x^3)/((x-1)*(x^2+x+1)*(x^2+4*x-1)).
 * https://oeis.org/A108929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=17, a(3)=71, a(4)=300 For n≥5:
 *            a(n)=4∙a(n-1)+a(n-2)+a(n-3)-4∙a(n-4)-a(n-5) (Derived from
 *            (1-x^4-2x^3)/((x-1)(x^2+x+1)(x^2+4x-1)) )
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t init[5] = {1,4,17,71,300};
    if (n < 5)
        return init[n];

    seq_t buf[5];
    for (int i = 0; i < 5; ++i)
        buf[i] = init[i];

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next =
            (seq_t)4 * buf[(i-1)%5] +
            buf[(i-2)%5] +
            buf[(i-3)%5] -
            (seq_t)4 * buf[(i-4)%5] -
            buf[(i-5)%5];
        buf[i%5] = next;
    }
    return buf[n%5];
}
