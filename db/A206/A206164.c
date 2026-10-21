/*
 * A206164  Number of (n+1) X 4 0..3 arrays with every 2 X 2 subblock in a row
 *          having an equal number of equal diagonal or equal antidiagonal
 *          elements, adjacent rows differing in this number, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A206164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 641 a(2) = 14182 a(3) = 463878 a(4) = 10599630 a(5) =
 *            345652310 a(6) = 7934434238 a(7) = 257597022598 For n > 7: a(n)
 *            = a(n-1) + 745·a(n-2) - 489·a(n-3) - 5808·a(n-4) + 1584·a(n-5) +
 *            10368·a(n-6)
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[7] = {
        (seq_t)641,          /* a(1) */
        (seq_t)14182,        /* a(2) */
        (seq_t)463878,       /* a(3) */
        (seq_t)10599630,     /* a(4) */
        (seq_t)345652310,    /* a(5) */
        (seq_t)7934434238,   /* a(6) */
        (seq_t)257597022598  /* a(7) */
    };

    if (n == (idx_t)0) return (seq_t)0;      /* undefined for n=0, safe guard */
    if (n <= (idx_t)7) return init[n - (idx_t)1]; /* return one of the 7 initial terms */

    /* Circular buffer holding the last six computed terms:
       last[0] = a(i-1), last[1] = a(i-2), ..., last[5] = a(i-6)    */
    seq_t last[6];
    last[0] = init[6]; /* a(7) */
    last[1] = init[5]; /* a(6) */
    last[2] = init[4]; /* a(5) */
    last[3] = init[3]; /* a(4) */
    last[4] = init[2]; /* a(3) */
    last[5] = init[1]; /* a(2) */

    for (idx_t i = (idx_t)8; i <= n; ++i)
    {
        seq_t new_val = last[0];
        new_val += (seq_t)745  * last[1];
        new_val -= (seq_t)489  * last[2];
        new_val -= (seq_t)5808 * last[3];
        new_val += (seq_t)1584 * last[4];
        new_val += (seq_t)10368 * last[5];

        /* shift the buffer so that last[0] becomes last[1], etc.      */
        last[5] = last[4];
        last[4] = last[3];
        last[3] = last[2];
        last[2] = last[1];
        last[1] = last[0];
        last[0] = new_val;
    }

    return last[0];   /* last[0] now holds a(n) */
}
