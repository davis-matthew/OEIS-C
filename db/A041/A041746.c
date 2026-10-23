/*
 * A041746  Numerators of continued fraction convergents to sqrt(393).
 * https://oeis.org/A041746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of the continued *
 *            fraction for sqrt(393). The convergents satisfy * p_{-2} = 0,
 *            p_{-1} = 1, * p_k = a_k * p_{k-1} + p_{k-2}, * where the
 *            continued fraction terms are * * a_0 = 19, * a_1..a_20 =
 *            {1,4,1,2,4,1,1,1,1,12,1,1,1,1,4,2,1,4,1,38}, * and repeat
 *            with...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 46, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial first term */
    if (n == 0) {
        return (seq_t)19;
    }

    /* Periodic part of the continued fraction (indices 1..20) */
    static const unsigned int cf[21] = {
        19,   /* a0 */
        1,    /* a1 */
        4,    /* a2 */
        1,    /* a3 */
        2,    /* a4 */
        4,    /* a5 */
        1,    /* a6 */
        1,    /* a7 */
        1,    /* a8 */
        1,    /* a9 */
        12,   /* a10 */
        1,    /* a11 */
        1,    /* a12 */
        1,    /* a13 */
        1,    /* a14 */
        4,    /* a15 */
        2,    /* a16 */
        1,    /* a17 */
        4,    /* a18 */
        1,    /* a19 */
        38    /* a20 (2*a0) */
    };

    /* p_{-2} and p_{-1} */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t p_curr  = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Determine the k-th continued fraction term a_k */
        unsigned int a_k;
        if (k <= 20) {
            a_k = cf[k];
        } else {
            /* For k >= 21 the terms repeat with period 20 (indices 1..20) */
            unsigned int idx = k % 20;
            if (idx == 0) idx = 20;           /* a_20 for multiples of 20 */
            a_k = cf[idx];
        }

        /* Compute p_k = a_k * p_{k-1} + p_{k-2} */
        p_curr = (seq_t)a_k * p_prev1 + p_prev2;

        /* Shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_curr;
}
