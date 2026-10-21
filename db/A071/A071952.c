/*
 * A071952  Diagonal T(n, 4) of triangle in A071951.
 * https://oeis.org/A071952
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = (-7*2^n + 405*6^n - 2268*12^n + 2500*20^n)/630, n ≥ 0
 *            Recurrence (derived from the denominator of the generating
 *            function): a(4) = 1, a(5) = 40, a(6) = 1092, a(7) = 25664 a(n) =
 *            40*a(n-1) - 508*a(n-2) + 2304*a(n-3) - 2880*a(n-4), n ≥ 8 The
 *            computation uses only integer arithmetic on seq_...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;            /* offset is 4; behaviour below is irrelevant */

    /* Convert the sequence index into the index for the underlying recurrence */
    idx_t k = n - 4;                 /* we need the k‑th term of the f sequence */

    /* Base values for f(0) … f(3)  (f(k) = a(k+4)) */
    const seq_t f0 = 1;
    const seq_t f1 = 40;
    const seq_t f2 = 1092;
    const seq_t f3 = 25664;

    if (k == 0) return f0;
    if (k == 1) return f1;
    if (k == 2) return f2;
    if (k == 3) return f3;

    /* Use the linear recurrence to generate successive terms in O(n) time */
    seq_t fm4 = f0;   /* f(k-4)   */
    seq_t fm3 = f1;   /* f(k-3)   */
    seq_t fm2 = f2;   /* f(k-2)   */
    seq_t fm1 = f3;   /* f(k-1)   */
    seq_t fk   = 0;   /* f(k)     */

    for (idx_t i = 4; i <= k; ++i) {
        /* f(k) = 40*f(k-1) - 508*f(k-2) + 2304*f(k-3) - 2880*f(k-4) */
        fk = fm1 * 40 - fm2 * 508 + fm3 * 2304 - fm4 * 2880;

        /* slide window for next iteration */
        fm4 = fm3;
        fm3 = fm2;
        fm2 = fm1;
        fm1 = fk;
    }

    return fk;
}
