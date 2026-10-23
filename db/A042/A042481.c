/*
 * A042481  Denominators of continued fraction convergents to sqrt(768).
 * https://oeis.org/A042481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(768) */
static const seq_t cycle[8] = {
    (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)13,
    (seq_t)2, (seq_t)2, (seq_t)1, (seq_t)54
};

seq_t a(idx_t n)
{
    seq_t prev2 = (seq_t)1;      /* q_{-2} */
    seq_t prev1 = (seq_t)0;      /* q_{-1} */
    seq_t curr  = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai;
        if (i == 0) {
            ai = (seq_t)27;      /* a_0 */
        } else {
            ai = cycle[(i - 1) % 8];   /* a_i for i>=1 */
        }

        /* multiply prev1 by ai using only addition to avoid overflow */
        seq_t prod = (seq_t)0;
        seq_t cnt  = ai;
        while (cnt > (seq_t)0) {
            prod += prev1;
            cnt -= (seq_t)1;
        }

        curr = prod + prev2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
