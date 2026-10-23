/*
 * A028049  Expansion of 1/((1-3*x)*(1-4*x)*(1-9*x)*(1-12*x)).
 * https://oeis.org/A028049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - 267*a(n-2) + 1008*a(n-3) - 1296*a(n-4) with
 *            a(0) = 1, a(1) = 28, a(2) = 517, a(3) = 8008
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return early for the first four terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;
    if (n == 2) return (seq_t)517;
    if (n == 3) return (seq_t)8008;

    /* keep the four most recent values */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)28;
    seq_t a2 = (seq_t)517;
    seq_t a3 = (seq_t)8008;
    seq_t an = a3;

    idx_t i;
    for (i = 4; i <= n; ++i) {
        /* compute 28*a3 + 1008*a1 and 267*a2 + 1296*a0 separately */
        seq_t t1 = (seq_t)28  * a3;
        seq_t t2 = (seq_t)1008 * a1;
        seq_t t3 = (seq_t)267  * a2;
        seq_t t4 = (seq_t)1296 * a0;

        /* difference is always non‑negative for the sequence */
        an = (t1 + t2) - (t3 + t4);

        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
