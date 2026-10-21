/*
 * A093197  Number of labeled plane 2-trees on n triangles.
 * https://oeis.org/A093197
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (2/3 * C(n) - 1/6 * C(n+1)) where C(n) is the n‑th
 *            Catalan number (A000108), and the sequence is defined for n ≥ 2
 *            (offset 2). The implementation computes the value exactly using
 *            only integer arithmetic with arbitrary‑precision support for
 *            seq_t
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 2. */
    if (n < 2) return (seq_t)0;

    /* -------- Compute n! -------- */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* -------- Compute Catalan numbers C(n) and C(n+1) -------- */
    /* Start with C(0) = 1 */
    seq_t catalan = 1;
    for (idx_t i = 0; i < n; ++i) {
        /* C(i+1) = C(i) * 2*(2*i+1) / (i+2)   (division exact) */
        seq_t mult = catalan * (seq_t)(2 * (2 * i + 1));
        catalan = mult / (seq_t)(i + 2);
    }
    /* Now catalan == C(n) */

    /* Compute C(n+1) using the same recurrence with i = n */
    seq_t catalan_next = catalan * (seq_t)(2 * (2 * n + 1));
    catalan_next /= (seq_t)(n + 2);   /* now catalan_next == C(n+1) */

    /* -------- Compute a(n) -------- */
    /* a(n) = n! * (4*C(n) - C(n+1)) / 6   (formula derived from above) */
    seq_t term = catalan * 4 - catalan_next;   /* 4*C(n) - C(n+1) */
    seq_t result = fact * term;
    result /= 6;

    return result;
}
