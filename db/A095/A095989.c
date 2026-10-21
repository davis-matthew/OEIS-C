/*
 * A095989  INVERTi transform applied to the ordered Bell numbers.
 * https://oeis.org/A095989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let B(n) be the ordered Bell numbers with B(0)=1. B(n) satisfies
 *            B(n) = Σ_{k=0}^{n-1} C(n,k) * B(k). Then a(n) = B(n) -
 *            Σ_{k=1}^{n-1} B(k) * a(n-k). This follows from the
 *            generating‑function identity A(x) = (1-1/B(x))/x
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
 
seq_t a(idx_t n)
{
    /* No definition for n=0 is required; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;
 
    static seq_t *B = NULL;   /* Ordered Bell numbers, B[0]=1          */
    static seq_t *A = NULL;   /* Sequence A095989 itself, A[0]=0      */
    static idx_t cap = 0;     /* Current capacity of the arrays      */
    static idx_t last = 0;    /* Largest index that has been computed*/
 
    /* Grow storage if necessary. */
    if (n > last) {
        /* Ensure capacity for indices 0..n. */
        if (n + 1 > cap) {
            idx_t newCap = cap ? cap << 1 : 2;
            while (newCap <= n)
                newCap <<= 1;
            B  = (seq_t*)realloc(B, newCap * sizeof(seq_t));
            A  = (seq_t*)realloc(A, newCap * sizeof(seq_t));
            cap = newCap;
        }
 
        if (last == 0) {      /* initialise
                                 * B[0] = 1, A[0] = 0  */
            B[0] = (seq_t)1;
            A[0] = (seq_t)0;
            last = 0;
        }
 
        /* Compute values up to n. */
        for (idx_t m = last + 1; m <= n; ++m) {
            seq_t b_m = (seq_t)0;
            seq_t binom = (seq_t)1;          /* binom(m,0) */
 
            /* Compute B(m) = Σ_{k=0}^{m-1} binom(m,k) * B(k) */
            b_m += binom * B[0];             /* k = 0 */
            for (idx_t k = 1; k < m; ++k) {
                binom = binom * (seq_t)(m - k + 1) / k;  /* C(m,k) */
                b_m += binom * B[k];
            }
            B[m] = b_m;
 
            /* Compute a(m) = B(m) - Σ_{k=1}^{m-1} B(k) * a(m-k) */
            seq_t a_m = b_m;
            for (idx_t k = 1; k < m; ++k)
                a_m -= B[k] * A[m - k];
            A[m] = a_m;
        }
 
        last = n;
    }
 
    return A[n];
}
