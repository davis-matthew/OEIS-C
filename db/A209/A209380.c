/*
 * A209380  1/4 the number of (n+1)X7 0..2 arrays with every 2X2 subblock
 *          having distinct edge sums.
 * https://oeis.org/A209380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 38*a(n-2) - 38*a(n-3) - 516*a(n-4) + 516*a(n-5)
 *            + 3144*a(n-6) - 3144*a(n-7) - 8480*a(n-8) + 8480*a(n-9) +
 *            7936*a(n-10) - 7936*a(n-11) Initial values: a(1)=128, a(2)=410,
 *            a(3)=840, a(4)=4004, a(5)=8648, a(6)=52784, a(7)=114056,
 *            a(8)=790712, a(9)=1693448, a(10)=12487208, a(11)=26563...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* We only need values for n ≥ 1.  For n ≤ 11 we return the
       pre‑computed constants.  For larger n we apply the linear
       recurrence. */
    static const seq_t init[11] = {
        128,        410,        840,        4004,        8648,
        52784,     114056,      790712,     1693448,    12487208,
        26563080
    };

    if (n == 0)
        return (seq_t)0;          /* undefined for n=0 but harmless */

    if (n <= 11)
        return init[n - 1];

    /* Sliding window of the last 11 terms: buf[0] = a(n-11),
       buf[10] = a(n-1).  We initialise with the first 11 terms. */
    static seq_t buf[11];
    for (int i = 0; i < 11; ++i)
        buf[i] = init[i];

    /* Compute a(k) for k = 12 .. n using the recurrence. */
    for (idx_t k = 12; k <= n; ++k)
    {
        seq_t t = buf[10];  /* a(k-1) */

        t += mul_small(buf[9],  38u);   /* +38*a(k-2) */
        t -= mul_small(buf[8],  38u);   /* -38*a(k-3) */
        t -= mul_small(buf[7], 516u);   /* -516*a(k-4) */
        t += mul_small(buf[6], 516u);   /* +516*a(k-5) */
        t += mul_small(buf[5], 3144u);  /* +3144*a(k-6) */
        t -= mul_small(buf[4], 3144u);  /* -3144*a(k-7) */
        t -= mul_small(buf[3], 8480u);  /* -8480*a(k-8) */
        t += mul_small(buf[2], 8480u);  /* +8480*a(k-9) */
        t += mul_small(buf[1], 7936u);  /* +7936*a(k-10) */
        t -= mul_small(buf[0], 7936u);  /* -7936*a(k-11) */

        /* Shift window to make room for the new term. */
        for (int i = 0; i < 10; ++i)
            buf[i] = buf[i + 1];
        buf[10] = t;  /* buf[10] now holds a(k) */
    }

    return buf[10];   /* a(n) */
}
