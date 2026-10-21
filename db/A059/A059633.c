/*
 * A059633  Expansion of g.f. x^3/(1 - 2*x + x^3 - x^4).
 * https://oeis.org/A059633
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 73, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 2*a(n-1) - a(n-3) + a(n-4) for n ≥ 7
   (initial values: a(3)=1, a(4)=2, a(5)=4, a(6)=7) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset starts at 3 – you may return 0 for n < 3 if you wish */
    if (n < 3) return 0;

    /* directly return the first four defined terms */
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)2;
    if (n == 5) return (seq_t)4;
    if (n == 6) return (seq_t)7;

    /* rolling variables for a(i-4), a(i-3), a(i-2), a(i-1) */
    seq_t a_i_minus_4 = (seq_t)1; /* a(3) */
    seq_t a_i_minus_3 = (seq_t)2; /* a(4) */
    seq_t a_i_minus_2 = (seq_t)4; /* a(5) */
    seq_t a_i_minus_1 = (seq_t)7; /* a(6) */
    seq_t a_i;                     /* will hold a(i) */

    /* iterate from i = 7 up to n */
    for (idx_t i = 7; i <= n; ++i)
    {
        /* a(i) = 2*a(i-1) - a(i-3) + a(i-4) */
        a_i = (a_i_minus_1 << 1) - a_i_minus_3 + a_i_minus_4;

        /* shift window for the next iteration */
        a_i_minus_4 = a_i_minus_3;
        a_i_minus_3 = a_i_minus_2;
        a_i_minus_2 = a_i_minus_1;
        a_i_minus_1 = a_i;
    }

    return a_i_minus_1; /* a(n) */
}
