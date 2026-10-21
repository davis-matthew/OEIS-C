/*
 * A059413  Number of distinct languages accepted by unary DFA's with n
 *          states.
 * https://oeis.org/A059413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 0,  a(1) = 2
   a(n) = 2*a(n‑1) + ψ(n)      (n ≥ 1)
   ψ(n) = 2^n – Σ_{d|n, d<n} ψ(d)   (primitive binary words) */

/* No external libraries are required; only the build‑provided types. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *pow2 = NULL;    /* powers of 2, pow2[i] = 2^i   */
    static seq_t *psi  = NULL;    /* primitive word counts       */
    static seq_t *A    = NULL;    /* sequence values             */
    static idx_t  max_n = 0;       /* highest n already computed  */
    static size_t cap   = 0;       /* current capacity of arrays   */

    if (n == 0) return (seq_t)0;  /* behaviour below offset is irrelevant */

    /* extend cache if necessary */
    if (n > max_n) {
        /* determine new capacity */
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cap) {
            cap = new_cap;
            /* reallocate each array */
            pow2 = (seq_t *)realloc(pow2, cap * sizeof(seq_t));
            psi  = (seq_t *)realloc(psi,  cap * sizeof(seq_t));
            A    = (seq_t *)realloc(A,    cap * sizeof(seq_t));
            /* initialise the added entries lazily below */
        }

        /* initialise pow2[0] if this is the very first allocation */
        if (max_n == 0) pow2[0] = (seq_t)1;

        /* compute values for indices max_n+1 … n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* powers of two */
            pow2[i] = pow2[i-1] + pow2[i-1];

            /* compute ψ(i) as 2^i minus sum of ψ(d) over proper divisors d of i */
            seq_t sum = (seq_t)0;
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    if (d < i) sum += psi[d];
                    idx_t d2 = i / d;
                    if (d2 != d && d2 < i) sum += psi[d2];
                }
            }
            psi[i] = pow2[i] - sum;

            /* sequence recurrence */
            A[i] = (A[i-1] << 1) + psi[i];
        }
        max_n = n;
    }

    return A[n];
}
