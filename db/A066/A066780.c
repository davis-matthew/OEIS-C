/*
 * A066780  a(n) = Product_{k=1..n} sigma(k); sigma(k) is the sum of the
 *          positive divisors of n.
 * https://oeis.org/A066780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} σ(k) (σ(k) = sum of positive divisors of k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *sigma = NULL;   /* σ values, 1‑based indexing */
static seq_t *prod  = NULL;   /* cumulative products, prod[0] = 1 */
static idx_t  max_n_computed = 0;

/* Compute A066780 term a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* not defined for n = 0, return 0 */

    /* If we need a larger n, recompute all values up to n */
    if (n > max_n_computed) {
        /* Allocate and zero new arrays */
        seq_t *s = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        seq_t *p = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!s || !p) {         /* allocation failed */
            if (s) free(s);
            if (p) free(p);
            /* In the unlikely event of OOM, return 0 */
            return (seq_t)0;
        }

        /* Compute σ(k) for k = 1..n by a simple divisor sieve */
        for (idx_t i = 1; i <= n; ++i) {
            seq_t val = (seq_t)i;          /* divisor to add */
            for (idx_t j = i; j <= n; j += i)
                s[j] += val;
        }

        /* Compute cumulative products prod[k] = ∏_{i=1..k} σ(i) */
        p[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            p[i] = p[i - 1] * s[i];

        /* Release old arrays */
        if (sigma) free(sigma);
        if (prod)  free(prod);

        sigma = s;
        prod  = p;
        max_n_computed = n;
    }

    return prod[n];
}
