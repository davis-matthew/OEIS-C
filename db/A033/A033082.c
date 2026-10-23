/*
 * A033082  Numbers in which all pairs of consecutive base-4 digits differ by
 *          2.
 * https://oeis.org/A033082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*a(n-3) + a(n-6) - 4*a(n-9) for n > 9; initial values:
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=7, a(5)=8, a(6)=13, a(7)=29,
 *            a(8)=34, a(9)=55
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 48, u64 n <= 96, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small optimisation: use the initial values directly. */
    if (n == (idx_t)0) return (seq_t)0;
    static const seq_t init[10] = {
        0,          /* dummy a(0) */
        1,          /* a(1) */
        2,          /* a(2) */
        3,          /* a(3) */
        7,          /* a(4) */
        8,          /* a(5) */
        13,         /* a(6) */
        29,         /* a(7) */
        34,         /* a(8) */
        55          /* a(9) */
    };
    if (n <= (idx_t)9) return init[(size_t)n];

    /* Circular buffer to hold the last 10 values (indices 0..9 modulo 10). */
    seq_t buf[10];
    for (size_t i = 1; i <= 9; ++i)
        buf[i % 10] = init[i];

    for (idx_t idx = 10; idx <= n; ++idx) {
        /* Compute a(idx) = 4*a(idx-3) + a(idx-6) - 4*a(idx-9) */
        seq_t val = buf[(size_t)((idx - 3) % 10)];   /* a(idx-3) */
        val <<= 2;                                   /* *4 */
        val += buf[(size_t)((idx - 6) % 10)];         /* + a(idx-6) */
        seq_t sub = buf[(size_t)((idx - 9) % 10)];    /* a(idx-9) */
        sub <<= 2;                                   /* *4 */
        val -= sub;                                  /* - 4*a(idx-9) */
        buf[(size_t)(idx % 10)] = val;               /* store a(idx) */
    }
    return buf[(size_t)(n % 10)];
}
