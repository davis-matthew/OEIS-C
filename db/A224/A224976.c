/*
 * A224976  L.g.f.: log( 1 + Sum_{n>=1} x^(n*(3*n-1)/2) + x^(n*(3*n+1)/2) ) =
 *          Sum_{n>=1} a(n)*x^n/n.
 * https://oeis.org/A224976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*A111932(n) - sigma(n) (OEIS A224976)
 * Range:     i8 n <= 71, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Implemented via series: a(n) = coefficient of x^{n-1} in (G'(x)/G(x))
   where G(x) = 1 + Σ_{k≥1} x^{k(3k±1)/2} ) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static arrays for the generating function */
static seq_t *F      = NULL; /* G(x) = 1 + Σ x^{p_k}   */
static seq_t *Fprime = NULL; /* G'(x) as polynomial   */
static seq_t *Inv    = NULL; /* 1/G(x) as series      */
static seq_t *H      = NULL; /* G'/G as series       */
static seq_t *Cache  = NULL; /* a(1)..a(n)             */
static idx_t MaxN    = 0;     /* largest n handled      */

/* Build all auxiliary arrays up to and including n */
static void build_tables(idx_t n)
{
    if (n <= MaxN) return;              /* already built */

    /* free any previous allocation */
    if (F)      free(F);
    if (Fprime) free(Fprime);
    if (Inv)    free(Inv);
    if (H)      free(H);
    if (Cache)  free(Cache);

    /* allocate arrays; explicit cast needed for C++ */
    idx_t size = n + 1;
    F      = (seq_t*)calloc(size, sizeof(seq_t));
    Fprime = (seq_t*)calloc(size, sizeof(seq_t));
    Inv    = (seq_t*)calloc(size, sizeof(seq_t));
    H      = (seq_t*)calloc(size, sizeof(seq_t));
    Cache  = (seq_t*)calloc(n,  sizeof(seq_t));  /* indices 0..n-1 */

    if (!F || !Fprime || !Inv || !H || !Cache) {
        /* allocation failure – abort (should not happen) */
        exit(EXIT_FAILURE);
    }

    /* G(x) starts with constant term 1 */
    F[0] = 1;

    /* Mark the generalized pentagonal numbers */
    for (idx_t k = 1; ; ++k) {
        idx_t g1 = k * (3*k - 1) / 2;   /* k(3k-1)/2 */
        idx_t g2 = k * (3*k + 1) / 2;   /* k(3k+1)/2 */
        if (g1 > n && g2 > n) break;

        if (g1 <= n) {
            F[g1] = 1;
            /* derivative coefficient: d/dx x^{g1} = g1 * x^{g1-1} */
            Fprime[g1-1] = (seq_t)g1;
        }
        if (g2 <= n) {
            F[g2] = 1;
            Fprime[g2-1] = (seq_t)g2;
        }
    }

    /* Compute 1/G(x) via series inversion */
    Inv[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= i; ++j) {
            if (F[j]) sum += F[j] * Inv[i - j];
        }
        Inv[i] = -sum;   /* because 1 + F*Inv = 0 for degree ≥1 */
    }

    /* Compute H(x) = G'(x)/G(x) as convolution of Fprime and Inv */
    for (idx_t j = 0; j <= n; ++j) {
        seq_t fp = Fprime[j];
        if (!fp) continue;
        for (idx_t i = j; i <= n; ++i) {
            H[i] += fp * Inv[i - j];
        }
    }

    /* Store the sequence values: a(n) = coefficient of x^{n-1} in H */
    for (idx_t i = 1; i <= n; ++i) {
        Cache[i - 1] = H[i - 1];
    }

    MaxN = n;   /* cache is valid up to n */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* sequence offset is 1 */
    build_tables(n);                    /* ensure tables up to n */
    return Cache[n - 1];
}
