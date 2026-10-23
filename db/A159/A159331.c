/*
 * A159331  Transform of the finite sequence (1, 0, -1, 0, 1, 0, -1) by the
 *          T_{1,1} transform (see link).
 * https://oeis.org/A159331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) for n≥9 a(0)=2, a(1)=4,
 *            a(2)=9, a(3)=23, a(4)=55, a(5)=126, a(6)=293, a(7)=680,
 *            a(8)=1581
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[9] = {
        (seq_t)2, (seq_t)4, (seq_t)9, (seq_t)23, (seq_t)55,
        (seq_t)126, (seq_t)293, (seq_t)680, (seq_t)1581
    };
    if (n < 9) return init[n];

    seq_t buf[9];
    for (size_t i = 0; i < 9; ++i) buf[i] = init[i];

    for (idx_t i = 9; i <= n; ++i) {
        buf[i % 9] = 3 * buf[(i - 1) % 9] - 2 * buf[(i - 2) % 9] + buf[(i - 3) % 9];
    }

    return buf[n % 9];
}
