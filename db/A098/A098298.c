/*
 * A098298  Member r=13 of the family of Chebyshev sequences S_r(n) defined in
 *          A092184.
 * https://oeis.org/A098298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - a(n-2) + 2 as given by OEIS A098298 * Initial
 *            values: a(0) = 0, a(1) = 1 * The recurrence is applied
 *            iteratively; only the two most recent terms * are stored,
 *            keeping memory usage O(1). All arithmetic uses the * definition
 *            of seq_t, so it works for built‑in 64‑bit types as well...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly. */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* a(0) and a(1) */
    seq_t a_prev2 = (seq_t)0;   /* a(n-2) */
    seq_t a_prev1 = (seq_t)1;   /* a(n-1) */
    seq_t a_curr  = (seq_t)0;   /* a(n) */

    /* Iteratively compute up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        a_curr = (seq_t)11 * a_prev1 - a_prev2 + (seq_t)2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;   /* a(n) */
}
