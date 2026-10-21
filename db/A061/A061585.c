/*
 * A061585  a(1) = 1, a(n)= number obtained by replacing each digit of a(n-1)
 *          with seven times its value.
 * https://oeis.org/A061585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(1)=1; for n>1, a(n) is obtained by replacing each
 *            decimal digit d of a(n‑1) with the two‑digit decimal
 *            representation of 7*d (if 7*d<10 it is a one‑digit number). The
 *            new digits are concatenated from most significant to least
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: replace each decimal digit d of x by 7*d and concatenate the
   results to form a new integer. */
static seq_t
transform(seq_t x)
{
    seq_t new_val = 0;          /* the accumulated new number             */
    seq_t pow10   = 1;          /* multiplier 10^k for the next part      */
    if (x == 0)                 /* nothing to process (should not happen) */
        return 0;

    while (x != 0) {
        /* get the least significant digit of x */
        unsigned digit = (unsigned)(x % 10u);
        x /= 10u;

        /* 7 * d may produce one or two decimal digits */
        unsigned prod = digit * 7u;      /* 0 <= prod <= 63 */
        unsigned len  = (prod < 10u) ? 1u : 2u;  /* number of decimal digits of prod */

        /* append prod to the accumulated value */
        new_val += (seq_t)prod * pow10;

        /* update pow10 for the next digit position */
        pow10 *= (len == 1u) ? (seq_t)10u : (seq_t)100u;
    }

    return new_val;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n <= 1u)
        return 1u;          /* a(1) = 1 */

    seq_t val = 1u;          /* current value, starts with a(1) */
    for (idx_t i = 2u; i <= n; ++i)
        val = transform(val);

    return val;
}
