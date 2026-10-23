/*
 * A123889  Expansion of g.f.: x/((1-x^2)^4 -1+x).
 * https://oeis.org/A123889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*4 - a(n-3)*6 + a(n-5)*4 - a(n-7) for n >= 7, with
 *            a(0)=1, a(1)=4, a(2)=16, a(3)=58, a(4)=208, a(5)=740, a(6)=2628
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

static const seq_t base_vals[7] = {
    (seq_t)1,
    (seq_t)4,
    (seq_t)16,
    (seq_t)58,
    (seq_t)208,
    (seq_t)740,
    (seq_t)2628
};

seq_t a(idx_t n)
{
    if (n < 7)
        return base_vals[n];

    /* Circular buffer of the last 8 values (indices 0..7). */
    seq_t buf[8];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = base_vals[i];

    /* Compute sequentially up to n. */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t val = 0;
        val += (seq_t)4 * buf[(i - 1) & 7];
        val -= (seq_t)6 * buf[(i - 3) & 7];
        val += (seq_t)4 * buf[(i - 5) & 7];
        val -=       buf[(i - 7) & 7];
        buf[i & 7] = val;
    }

    return buf[n & 7];
}
