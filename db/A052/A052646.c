/*
 * A052646  E.g.f. 1/((1-x)(1-x-x^2)).
 * https://oeis.org/A052646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * c_n, where c_0 = 1, c_1 = 2, c_2 = 4 and for n ≥ 3,
 *            c_n = 2 · c_{n-1} – c_{n-3} (the coefficients of
 *            1/((1‑x)(1‑x‑x²)) in the egf)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0) return (seq_t)1;

    /* Base values of the recurrence */
    seq_t c0 = (seq_t)1;   /* c_0 */
    seq_t c1 = (seq_t)2;   /* c_1 */
    seq_t c2 = (seq_t)4;   /* c_2 */

    /* Compute n! */
    seq_t fac = (seq_t)1;
    for (idx_t k = 2; k <= n; ++k)
        fac = fac * (seq_t)k;

    /* Compute c_n up to n */
    if (n >= 3) {
        for (idx_t k = 3; k <= n; ++k) {
            seq_t c_new = (seq_t)2 * c2 - c0; /* 2*c_{n-1} - c_{n-3} */
            c0 = c1;
            c1 = c2;
            c2 = c_new;
        }
    }

    /* For n==1 or n==2 the recurrence loop is skipped, c1 or c2 is used directly */
    return fac * (n == 1 ? c1 : (n == 2 ? c2 : c2));
}
