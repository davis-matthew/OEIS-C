/*
 * A085023  E.g.f. satisfies exp(x*Sum_{n>=0} floor(a(n)/n!)*x^n) = Sum_{n>=0}
 *          a(n)*x^n/n!.
 * https://oeis.org/A085023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A085023
 * Recurrence: a(0)=1, and for n≥1
 *   a(n) = Σ_{k=0}^{n-1} binomial(n-1,k)·(k+1)!·⌊a(k)/k!⌋·a(n-1-k)
 *
 * Here ⌊a(k)/k!⌋ is stored in f[k] for reuse.
 */

#include <stddef.h>          /* size_t */
#include <stdlib.h>          /* malloc, realloc, free */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k) */

static void ensure_capacity(idx_t n, seq_t **A, seq_t **fact, seq_t **f, size_t *cap)
{
    if (n + 1 <= *cap) return;
    size_t new_cap = (*cap == 0) ? 64 : 2 * (*cap);
    if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
    *A    = (seq_t *)realloc(*A,    new_cap * sizeof(seq_t));
    *fact = (seq_t *)realloc(*fact, new_cap * sizeof(seq_t));
    *f    = (seq_t *)realloc(*f,    new_cap * sizeof(seq_t));
    *cap   = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *A    = NULL;   /* a[0..] */
    static seq_t *fact = NULL;   /* factorials */
    static seq_t *f    = NULL;   /* floor(a(k)/k!) */
    static idx_t  sz   = 0;      /* next free index, number of known terms */
    static size_t cap  = 0;

    /* Initialise on first call */
    if (sz == 0) {
        cap = 64;
        A   = (seq_t *)malloc(cap * sizeof(seq_t));
        fact = (seq_t *)malloc(cap * sizeof(seq_t));
        f    = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!A || !fact || !f) { /* allocation failure, return 0 as safety */
            if (A) free(A);
            if (fact) free(fact);
            if (f) free(f);
            return (seq_t)0;
        }
        A[0]   = (seq_t)1;   /* a(0) = 1 */
        fact[0] = (seq_t)1;  /* 0! = 1 */
        f[0]   = (seq_t)1;   /* ⌊a(0)/0!⌋ = 1 */
        sz = 1;
    }

    /* Compute up to n if needed */
    if (n >= sz) {
        for (idx_t i = sz; i <= n; ++i) {
            ensure_capacity((size_t)i, &A, &fact, &f, &cap);

            /* factorial i */
            fact[i] = fact[i-1] * (seq_t)i;

            /* recurrence for a[i] */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                /* term = binomial(i-1,k) * (k+1)! * f[k] * A[i-1-k] */
                seq_t term = binomial(i-1, k);
                term       = term * fact[k+1];     /* (k+1)! */
                term       = term * f[k];
                term       = term * A[i-1-k];
                sum        = sum + term;
            }
            A[i] = sum;

            /* f[i] = ⌊a[i]/i!⌋ */
            f[i] = A[i] / fact[i];
        }
        sz = n + 1;
    }

    return A[n];
}
