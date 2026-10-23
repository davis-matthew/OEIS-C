/*
 * A183713  1/20 of the number of (n+1) X 4 0..4 arrays with every 2 X 2
 *          subblock strictly increasing clockwise or counterclockwise with
 *          one decrease.
 * https://oeis.org/A183713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 10*a(n-2) - 10*a(n-4) - 2*a(n-5) + a(n-6)
 *            initial values: a(1)=12, a(2)=54, a(3)=224, a(4)=950, a(5)=4012,
 *            a(6)=16964
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline seq_t base_value(idx_t k)
{
    switch (k) {
        case 1: return (seq_t)12;
        case 2: return (seq_t)54;
        case 3: return (seq_t)224;
        case 4: return (seq_t)950;
        case 5: return (seq_t)4012;
        case 6: return (seq_t)16964;
    }
    return (seq_t)0;  /* never reached for valid k */
}

seq_t a(idx_t n)
{
    if (n <= 6)
        return base_value(n);

    /* Circular buffer holding the last six values:
       buf[0]=a(k-5), buf[1]=a(k-4), ..., buf[5]=a(k-1) */
    seq_t buf[6];
    buf[0] = base_value(1);
    buf[1] = base_value(2);
    buf[2] = base_value(3);
    buf[3] = base_value(4);
    buf[4] = base_value(5);
    buf[5] = base_value(6);

    for (idx_t k = 7; k <= n; ++k) {
        seq_t val = 2 * buf[5] + 10 * buf[4];
        val -= 10 * buf[2];
        val -= 2 * buf[1];
        val += buf[0];

        /* shift buffer left */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = val;
    }
    return buf[5];
}
