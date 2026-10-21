/*
 * A217444  a(n) = A(n)*7^(-floor(n+1)/3), where A(n) = 7*A(n-1) - 14*A(n-2) +
 *          7*A(n-3) with A(0)=0, A(1)=1, A(2)=7.
 * https://oeis.org/A217444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 32, u64 n <= 63, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A217444: a(n) = 10*a(n-3) - 17*a(n-6) + a(n-9)
 * Initial terms: a(0)=0, a(1)=1, a(2)=1, a(3)=5, a(4)=22, a(5)=13,
 *                a(6)=52, a(7)=204, a(8)=113
 */

#include <stddef.h>

static const seq_t base_vals[9] = {
    (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)22,
    (seq_t)13, (seq_t)52, (seq_t)204, (seq_t)113
};

seq_t a(idx_t n)
{
    if (n < (idx_t)9)
        return base_vals[n];

    /* A ring buffer holding the last ten computed values.  There are
       at most nine dependencies, so ten slots are enough to preserve
       all values needed for future computations.  */
    seq_t buf[10];
    for (size_t i = 0; i < 9; ++i)
        buf[i] = base_vals[i];

    for (idx_t i = (idx_t)9; i <= n; ++i)
    {
        size_t idx = (size_t)i % 10;
        size_t i3  = (size_t)(i - 3) % 10;
        size_t i6  = (size_t)(i - 6) % 10;
        size_t i9  = (size_t)(i - 9) % 10;

        buf[idx] = 10 * buf[i3] - 17 * buf[i6] + buf[i9];
    }

    return buf[(size_t)n % 10];
}
