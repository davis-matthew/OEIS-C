/*
 * A219803  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical, diagonal or antidiagonal
 *          neighbors in a random, but sorted with lexicographically
 *          nondecreasing rows and nonincreasing columns, 0..3 2 X n array.
 * https://oeis.org/A219803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) with a(1)=4, a(2)=4, a(3)=10, a(4)=21, a(5)=39, a(6)=68,
 *            a(7)=114, a(8)=186
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t base_seq[9] = {
    /* 0 unused  */ (seq_t)0,
    (seq_t)4,  /* a(1) */
    (seq_t)4,  /* a(2) */
    (seq_t)10, /* a(3) */
    (seq_t)21, /* a(4) */
    (seq_t)39, /* a(5) */
    (seq_t)68, /* a(6) */
    (seq_t)114,/* a(7) */
    (seq_t)186 /* a(8) */
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined, but safe default */
    if (n <= 8) return base_seq[(idx_t)n];

    /* initialise the last six values a(n-6)..a(n-1) for n = 9 */
    seq_t a1 = base_seq[3]; /* a(3) */
    seq_t a2 = base_seq[4]; /* a(4) */
    seq_t a3 = base_seq[5]; /* a(5) */
    seq_t a4 = base_seq[6]; /* a(6) */
    seq_t a5 = base_seq[7]; /* a(7) */
    seq_t a6 = base_seq[8]; /* a(8) */

    for (idx_t i = 9; i <= n; ++i)
    {
        /* next = 6*a6 - 15*a5 + 20*a4 - 15*a3 + 6*a2 - a1
           computed as (6*a6 + 20*a4 + 6*a2) - (15*a5 + 15*a3 + a1) to avoid negative
           intermediates for unsigned seq_t                           */
        seq_t pos = (seq_t)6 * a6 + (seq_t)20 * a4 + (seq_t)6 * a2;
        seq_t neg = (seq_t)15 * a5 + (seq_t)15 * a3 + a1;
        seq_t next = pos - neg;

        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }
    return a6;
}
