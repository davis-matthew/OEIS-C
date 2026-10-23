/*
 * A153294  G.f.: A(x) = F(x*F(x)^2) = F(F(x)-1) where F(x) = 1 + x*F(x)^2 is
 *          the g.f. of A000108 (Catalan).
 * https://oeis.org/A153294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A153294: a(0)=1, a(n)=Σ_{k=1..n} Catalan(k)·C(2n,n‑k)·k/n   (n>0)
 *  Catalan(k)=C(2k,k)/(k+1) computed iteratively;  
 *  Binomial C(2n,n‑k) updated by a simple recurrence.
 *  All computations use seq_t for arbitrary precision.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute C(2n,n) once. */
    seq_t binom = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
        binom = binom * ((seq_t)(n + k)) / (k);

    seq_t sum     = (seq_t)0;
    seq_t catalan = (seq_t)1;        /* Catalan(0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update Catalan(k) from Catalan(k‑1) */
        catalan = catalan * ((seq_t)(4 * (k - 1) + 2)) / ((seq_t)(k + 1));

        /* Update binom to C(2n,n‑k) from C(2n,n‑(k‑1)) */
        binom = binom * ((seq_t)(n - (k - 1))) / ((seq_t)(n + (k - 1) + 1));

        /* term = Catalan(k)·binom·k/n  (exact division) */
        seq_t term = catalan * binom * (seq_t)k / (seq_t)n;
        sum += term;
    }

    return sum;
}
