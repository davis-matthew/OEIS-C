/*
 * A207067  Number of n X 7 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*C(n+6,7) + 713*C(n+5,7) - 186*C(n+4,7) - 837*C(n+3,7)
 *            + 341*C(n+2,7)
 * Range:     u8 none, u16 n <= 2, u32 n <= 20, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(m,k) is the binomial coefficient m choose k. */

#include <stdint.h>

/* compute C(m,7)  for m >= 0; if m < 7 the result is 0.
   The calculation is performed multiplicatively to avoid large
   factorials, and the division by 7! (=5040) is exact. */
static seq_t binom7(idx_t m)
{
    if (m < 7) return 0;

    seq_t prod = 1;
    /* multiply (m)*(m-1)*...*(m-6) */
    for (idx_t i = 0; i < 7; ++i)
    {
        prod *= (seq_t)(m - i);
    }

    /* divide by 7! = 5040 ; the division is exact for integers */
    prod /= 5040;
    return prod;
}

/* OEIS sequence A207067: number of n × 7 binary arrays avoiding
   horizontal 001 and 010, and vertical 001 and 101. */
seq_t a(idx_t n)
{
    seq_t t1 = 31  * binom7(n + 6);
    seq_t t2 = 713 * binom7(n + 5);
    seq_t t3 = 341 * binom7(n + 2);
    seq_t t4 = 186 * binom7(n + 4);   /* to be subtracted */
    seq_t t5 = 837 * binom7(n + 3);   /* to be subtracted */

    return t1 + t2 + t3 - t4 - t5;
}
