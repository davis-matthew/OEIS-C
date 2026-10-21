/*
 * A230801  Number of n X 3 0..3 arrays x(i,j) with each element horizontally
 *          or vertically next to at least one element with value (x(i,j)+1)
 *          mod 4 and at least one element with value (x(i,j)-1) mod 4, and
 *          upper left element zero.
 * https://oeis.org/A230801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 2, a(3) = 14, a(4) = 40, a(5) = 298, a(6) =
 *            1546, a(7) = 8704, a(8) = 50450, a(9) = 286426, a(10) = 1650968,
 *            a(11) = 9481842, a(12) = 54537066, a(13) = 313793424 ; For n ≥
 *            14: a(n) = 8*a(n-1) - 7*a(n-2) - 40*a(n-3) + 3*a(n-4) +
 *            221*a(n-5) - 237*a(n-6) - 55*a(n-7) + 182*a(n-8) - 2...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation evaluates the 13‑term linear recurrence in O(n)
    time and uses a fixed circular buffer of 13 values.  The helper
    function mul_small from the utilities is used to avoid overflow on
    multiplication by the small integer coefficients.  It works for all
    supported seq_t types (uint8_t … arbitrary‑precision). */
#include <stdint.h>
#include "../utils/mul_small.h"

static const seq_t init[13] = {
    0ULL,          /* a(1) */
    2ULL,          /* a(2) */
    14ULL,         /* a(3) */
    40ULL,         /* a(4) */
    298ULL,        /* a(5) */
    1546ULL,       /* a(6) */
    8704ULL,       /* a(7) */
    50450ULL,      /* a(8) */
    286426ULL,     /* a(9) */
    1650968ULL,    /* a(10) */
    9481842ULL,    /* a(11) */
    54537066ULL,   /* a(12) */
    313793424ULL   /* a(13) */
};

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    if (n <= 13)
        return init[n - 1];

    /* buffer holds the last 13 computed terms */
    seq_t buf[13];
    for (int i = 0; i < 13; ++i)
        buf[i] = init[i];

    for (idx_t i = 14; i <= n; ++i) {
        seq_t term = mul_small(buf[12], 8);
        term = term - mul_small(buf[11], 7);
        term = term - mul_small(buf[10], 40);
        term = term + mul_small(buf[9], 3);
        term = term + mul_small(buf[8], 221);
        term = term - mul_small(buf[7], 237);
        term = term - mul_small(buf[6], 55);
        term = term + mul_small(buf[5], 182);
        term = term - mul_small(buf[4], 270);
        term = term + mul_small(buf[3], 324);
        term = term - mul_small(buf[2], 172);
        term = term + mul_small(buf[1], 112);
        term = term - mul_small(buf[0], 64);

        /* Shift buffer left and insert new term */
        for (int j = 0; j < 12; ++j)
            buf[j] = buf[j + 1];
        buf[12] = term;
    }

    return buf[12];
}
