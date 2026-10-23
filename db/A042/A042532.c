/*
 * A042532  Numerators of continued fraction convergents to sqrt(795).
 * https://oeis.org/A042532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 13252*a(n-4) - a(n-8) a(0) = 28, a(1) = 141, a(2) = 1297,
 *            a(3) = 6626, a(4) = 372353, a(5) = 1868391, a(6) = 17187872,
 *            a(7) = 87807751
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 8 terms */
    static const seq_t init[8] = {
        28, 141, 1297, 6626,
        372353, 1868391, 17187872, 87807751
    };

    /* Handle the offset 0..7 directly */
    if (n < 8)
        return init[n];

    /* Circular buffer for 8 consecutive terms.
       arr[i] holds a(i-7) ... arr[(i-1)%8] holds a(i-1). */
    seq_t arr[8];
    for (size_t i = 0; i < 8; ++i)
        arr[i] = init[i];

    idx_t i;
    for (i = 8; i <= n; ++i) {
        idx_t idx4 = (i - 4) & 7;   /* same as (i - 4) % 8 */
        idx_t idx8 = (i - 8) & 7;   /* same as (i - 8) % 8 */
        seq_t next = (seq_t)13252 * arr[idx4] - arr[idx8];
        arr[i & 7] = next;  /* same as arr[i % 8] */
    }

    return arr[n & 7];
}
