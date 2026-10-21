/*
 * A087350  a(n) = Sum_{k=0..n} (3*n)!/(3*k)!.
 * https://oeis.org/A087350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (3n)!/(3k)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A087350.
   The value may be arbitrarily large – all calculations are done in the
   seq_t type, which can be an unsigned integer or an
   arbitrary‑precision big‑integer class. */
seq_t a(idx_t n)
{
    /* term(k) = (3n)! / (3k)!  (k = current index) */
    seq_t term = 1;           /* term(n) = 1   */
    seq_t sum  = 1;           /* account for term(n) */

    for (idx_t k = n; k > 0; --k) {
        /* multiply by (3k-2)(3k-1)(3k) to obtain term(k-1) */
        term *= (seq_t)(3 * k - 2);
        term *= (seq_t)(3 * k - 1);
        term *= (seq_t)(3 * k);
        sum += term;
    }
    return sum;
}
