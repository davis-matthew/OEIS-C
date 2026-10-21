/*
 * A207258  Number of 7 X n 0..1 arrays avoiding 0 0 1 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) for n>8 Initial values: a(1) =
 *            42 a(2) = 1764 a(3) = 14406 a(4) = 64176 a(5) = 206514 a(6) =
 *            541380 a(7) = 1231650 a(8) = 2524704
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial eight terms of the sequence. */
    static const seq_t init[8] = {
        (seq_t)42,        /* a(1) */
        (seq_t)1764,      /* a(2) */
        (seq_t)14406,     /* a(3) */
        (seq_t)64176,     /* a(4) */
        (seq_t)206514,    /* a(5) */
        (seq_t)541380,    /* a(6) */
        (seq_t)1231650,   /* a(7) */
        (seq_t)2524704    /* a(8) */
    };

    if (n <= 8u)
        return init[n - 1];

    /* Ring buffer holding the last eight terms:
       buf[0] = a(k-1), buf[1] = a(k-2) ... buf[7] = a(k-8) */
    seq_t buf[8] = {
        init[7], init[6], init[5], init[4],
        init[3], init[2], init[1], init[0]
    };

    seq_t result = 0;           /* will hold a(n) at the end */

    for (idx_t k = 9; k <= n; ++k) {
        /* Compute the next term using signed 128‑bit intermediate */
        __int128 val = ( __int128)buf[0] * 8
                       - ( __int128)buf[1] * 28
                       + ( __int128)buf[2] * 56
                       - ( __int128)buf[3] * 70
                       + ( __int128)buf[4] * 56
                       - ( __int128)buf[5] * 28
                       + ( __int128)buf[6] * 8
                       - ( __int128)buf[7] * 1;

        seq_t ak = (seq_t)val;  /* result fits in seq_t for all n requested */

        /* Shift the ring buffer to make room for ak */
        for (int i = 7; i > 0; --i)
            buf[i] = buf[i - 1];
        buf[0] = ak;

        result = ak;           /* keep the latest term to return later */
    }

    return result;
}
