/*
 * A178750  Partial sums of floor(5^n/9).
 * https://oeis.org/A178750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((5*5^n - 18*n - 5)/36)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power5.h"

static const seq_t init[6] = {
    (seq_t)0,
    (seq_t)0,
    (seq_t)2,
    (seq_t)15,
    (seq_t)84,
    (seq_t)431
};

seq_t a(idx_t n)
{
    if (n < 6) return init[n];

    /* 64‑bit build: use unsigned __int128 for intermediate calculations. */
    if (sizeof(seq_t) <= sizeof(uint64_t))
    {
        unsigned __int128 pow = 1;   /* 5^0 */
        for (idx_t i = 1; i <= n; ++i)
            pow *= 5;

        unsigned __int128 term = pow * 5;                /* 5^{n+1} */
        unsigned __int128 sub  = (unsigned __int128)(18) * (unsigned __int128)n + 5;
        term -= sub;

        return (seq_t)(term / 36);
    }

    /* Arbitrary‑precision build: use the provided big integer routine. */
    seq_t pow5n = power5(n);        /* 5^n  */
    seq_t term  = pow5n * 5;        /* 5^{n+1} */
    seq_t sub   = (seq_t)((unsigned __int128)(18) * (unsigned __int128)n + 5);
    seq_t adj   = term - sub;
    return adj / 36;
}
