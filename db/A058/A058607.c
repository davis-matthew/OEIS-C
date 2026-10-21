/*
 * A058607  a(n) = (1 + 1/2 + 1/3 + ... + 1/n)*(2n-1)!/(n-1)!.
 * https://oeis.org/A058607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1,
 * a(n)=2*(2n-1)*a(n-1)+C(2n-1,n)*(n-1)!   for n≥2 */

#include <stdbool.h>

/* Compute the OEIS sequence A058607.
 * The value is returned as type seq_t, whose definition is supplied
 * by the build system.  idx_t is an unsigned 64‑bit integer type.  */
seq_t a(idx_t n)
{
    /* Off‑by‑zero handling: the sequence is defined for n≥1 only. */
    if (n == 0)
        return 0;   /* value below the offset is unspecified */

    if (n == 1)
        return (seq_t)1;

    /* Partial sums for the recurrence   */
    seq_t a_prev = (seq_t)1;               /* a(1)          */
    seq_t bin_prev = (seq_t)1;             /* C(1,1)        */
    seq_t fact_prev = (seq_t)1;            /* (1‑1)! = 0!   */

    for (idx_t k = 2; k <= n; ++k) {
        /* Update factorial (k‑1)! = previous * (k‑1)   */
        fact_prev = fact_prev * ((seq_t)(k - 1));

        /* Compute C(2k‑1,k) from C(2(k‑1)‑1,k‑1) */
        /* C(2k‑1,k) = C(2k‑3,k‑1) * (2k‑1)(2k‑2) / ((k‑1)k)   */
        seq_t bin = bin_prev * ((seq_t)(2 * k - 1));
        bin = bin * ((seq_t)(2 * k - 2));
        idx_t den = (k - 1) * k;            /* exact division */
        bin = bin / den;

        /* Compute a(k) via the recurrence             */
        seq_t term = a_prev * ((seq_t)(2 * (2 * k - 1)));  /* 2*(2k‑1)*a(k‑1) */
        term += bin * fact_prev;                                       /* + C(2k‑1,k)*(k‑1)! */

        /* Prepare for next iteration                 */
        a_prev = term;
        bin_prev = bin;
    }

    return a_prev;
}
