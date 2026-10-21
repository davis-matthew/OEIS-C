/*
 * A229736  Expansion of
 *          x^5*(2-5*x+3*x^2-x^3)/((1-x)^2*(1-2*x)^2*(1-3*x+x^2)).
 * https://oeis.org/A229736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   linear recurrence derived from the generating function a(n) =
 *            9*a(n-1) - 32*a(n-2) + 57*a(n-3) - 53*a(n-4) + 24*a(n-5) -
 *            4*a(n-6) for all n ≥ 9. Initial values (n = 0 … 8): 0, 0, 0, 0,
 *            0, 2, 13, 56, 201
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed initial terms up to n=8 */
    static const seq_t init[9] = {
        0, 0, 0, 0, 0, 2, 13, 56, 201
    };

    if (n < 9u)
        return init[n];

    /* Window of the 6 most recent terms for the recurrence:
       a0 = a(n-6), a1 = a(n-5), … , a5 = a(n-1)            */
    seq_t a0 = init[3];  /* a(3) */
    seq_t a1 = init[4];  /* a(4) */
    seq_t a2 = init[5];  /* a(5) */
    seq_t a3 = init[6];  /* a(6) */
    seq_t a4 = init[7];  /* a(7) */
    seq_t a5 = init[8];  /* a(8) */

    seq_t ai = 0;   /* will hold a(n) */

    for (idx_t i = 9; i <= n; ++i) {
        /* recurrence for n >= 9 */
        ai = 9 * a5 - 32 * a4 + 57 * a3 - 53 * a2 + 24 * a1 - 4 * a0;

        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = ai;
    }
    return ai;
}
