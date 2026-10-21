/*
 * A206568  Expand 1/(8 - 8 x + 3 x^3 - 2 x^4) in powers of x, then multiply
 *          coefficient of x^n by 8^(1 + floor(n/3)) to get integers.
 * https://oeis.org/A206568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=5. For n≥4: r = n mod 3 if (r == 0)
 *            a(n) = 8*a(n-1) - 3*a(n-3) + 16*a(n-4) else a(n) = a(n-1) -
 *            3*a(n-3) + 2*a(n-4)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 41, u64 n <= 80, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are provided by the build system.  */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)5;

    /* Circular buffer holding the last four values:
         indices: 0 -> a(k-4), 1 -> a(k-3), 2 -> a(k-2), 3 -> a(k-1)   */
    seq_t buf[4] = { (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)5 };
    /* We start computing from n = 4 up to the requested n. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t val;
        idx_t r = i % 3;
        /* a(i-1) is at buf[(i-1)%4] */
        seq_t a1 = buf[(i-1) % 4];
        /* a(i-3) is at buf[(i-3)%4] */
        seq_t a3 = buf[(i-3) % 4];
        /* a(i-4) is at buf[(i-4)%4] */
        seq_t a4 = buf[(i-4) % 4];

        if (r == 0) {
            /* a(i) = 8*a(i-1) - 3*a(i-3) + 16*a(i-4) */
            val = a1 * (seq_t)8   - a3 * (seq_t)3 + a4 * (seq_t)16;
        } else {
            /* a(i) = a(i-1) - 3*a(i-3) + 2*a(i-4) */
            val = a1 - a3 * (seq_t)3 + a4 * (seq_t)2;
        }
        /* Store the new value in the buffer for future use. */
        buf[i % 4] = val;
    }

    /* After the loop, a(n) is stored at buf[n % 4]. */
    return buf[n % 4];
}
