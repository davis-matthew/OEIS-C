/*
 * A157328  Expansion of 1/(1-2x*c(4x)) with c(x) g.f. of Catalan numbers
 *          (A000108).
 * https://oeis.org/A157328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n*A064062(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1,
   a(n)=2*∑_{k=0}^{n-1} Catalan(k)*4^k*a(n-1-k)   (n≥1)   */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static tables for memoization */
    static seq_t *A       = NULL;   /* values a(i)   */
    static seq_t *Catalan = NULL;   /* Catalan(k)    */
    static seq_t *P4      = NULL;   /* 4^k           */
    static size_t allocated = 0;    /* allocated size */
    static size_t computed  = 0;    /* number of entries already computed */

    /* Ensure tables are large enough for indices 0…n */
    size_t needed = (size_t)n + 1;
    if (allocated < needed) {
        seq_t *newA      = (seq_t *)realloc(A, needed * sizeof(seq_t));
        seq_t *newCatal = (seq_t *)realloc(Catalan, needed * sizeof(seq_t));
        seq_t *newP4     = (seq_t *)realloc(P4, needed * sizeof(seq_t));
        if (!newA || !newCatal || !newP4) return 0; /* allocation failure */

        A       = newA;
        Catalan = newCatal;
        P4      = newP4;

        /* Initialise new slots */
        for (size_t i = allocated; i < needed; ++i) {
            A[i]       = 0;
            Catalan[i] = 0;
            P4[i]      = 0;
        }
        allocated = needed;
    }

    /* Base case */
    if (computed == 0) {
        A[0]       = 1;
        Catalan[0] = 1;
        P4[0]      = 1;
        computed   = 1;
    }

    /* Pre‑compute Catalan and powers of 4 up to n */
    for (size_t i = computed; i < needed; ++i) {
        /* Catalan[i] = Catalan[i-1] * 2*(2*(i-1)+1) / (i+1) */
        seq_t mult = (seq_t)(2 * (2 * (i - 1) + 1));
        Catalan[i] = (Catalan[i - 1] * mult) / (i + 1);

        /* P4[i] = P4[i-1] * 4 */
        P4[i] = P4[i - 1] * 4;

        /* Compute a[i] using the convolution recurrence */
        seq_t sum = 0;
        for (size_t k = 0; k < i; ++k) {
            seq_t term = A[i - 1 - k];
            term *= 2;
            term *= Catalan[k];
            term *= P4[k];
            sum += term;
        }
        A[i] = sum;
    }

    computed = needed;
    return A[n];
}
