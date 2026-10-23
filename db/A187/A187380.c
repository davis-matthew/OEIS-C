/*
 * A187380  Number of 6-step S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 183*n^2 - 1035*n + 1432 for n > 4 Initial values: a(1)=0,
 *            a(2)=0, a(3)=24, a(4)=264, a(5)=832, a(6)=1810, a(7)=3154 For n
 *            <= 7 the values are taken from the table
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* hard‑coded small values */
    static const seq_t init[] = {
        (seq_t)0,   /* a(1) */
        (seq_t)0,   /* a(2) */
        (seq_t)24,  /* a(3) */
        (seq_t)264, /* a(4) */
        (seq_t)832, /* a(5) */
        (seq_t)1810,/* a(6) */
        (seq_t)3154 /* a(7) */
    };
    if (n <= 7)
        return init[n-1];

    /* quadratic formula for n > 4 (valid for all n >= 8 as well) */
    seq_t sq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t term1 = (seq_t)183 * sq;           /* 183*n^2 */
    seq_t term2 = (seq_t)1035 * (seq_t)n;    /* 1035*n */
    seq_t res = term1 - term2 + (seq_t)1432; /* 183*n^2 - 1035*n + 1432 */
    return res;
}
