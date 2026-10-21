/*
 * A220655  For n with a unique factorial base representation n = du*u! + ...
 *          + d2*2! + d1*1! (each di in range 0..i, cf. A007623), a(n) =
 *          (du+1)*u! + ... + (d2+1)*2! + (d1+1)*1!; a(n) = n +
 *          A007489(A084558(n)).
 * https://oeis.org/A220655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ∑_{i=1}^{m} i! where m! ≤ n < (m+1)!
 * Range:     u8 n <= 119, u16 n <= 5039, u32 n <= 5039, u64 n <= 5039,
 *            u128 n <= 5039, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A220655.
   The argument n is 1‑based; for n = 0 the function returns 0
   (this case never occurs for the intended offset). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety guard – offset is 1   */

    seq_t N   = (seq_t)n;                /* cast once – all arithmetic uses seq_t   */
    seq_t sum = 0;                       /* ∑ i! for i! ≤ N  */
    seq_t fact = 1;                      /* start with 0!  */

    for (idx_t i = 1; ; ++i) {
        /* Stop before the next factorial would overflow or exceed N. */
        if (fact > N / (seq_t)i) break;   /* next i! > N   */
        fact = fact * (seq_t)i;           /* compute i!    */
        /* The check above guarantees fact ≤ N, so no overflow occurs: */
        sum += fact;
    }

    return N + sum;
}
