/*
 * A127416  a(n) = Sum_{1<=k<=n, gcd(k,n)=1} (Sum_{i=1..k} gcd(i, k)).
 * https://oeis.org/A127416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1<=k<=n, gcd(k,n)=1} (Sum_{i=1..k} gcd(i,k))
 * Range:     u8 n <= 16, u16 n <= 190, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = Sum_{1<=k<=n, gcd(k,n)=1} A018804(k) where
 *           A018804(k) = Sum_{d|k} d * phi(k/d)  (phi = Euler's totient) */

#include <stdlib.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b) */

static seq_t *A = NULL;      /* A[k] = Sum_{i=1..k} gcd(i,k) */
static idx_t *phi = NULL;    /* Euler's totient for k */
static idx_t limit = 0;      /* largest n already computed */

/* Recompute phi[1..n] and A[1..n] from scratch whenever a larger n is requested */
static void recompute_up_to(idx_t n)
{
    if (n <= limit)
        return;

    /* Reallocate arrays to accommodate up to n */
    phi = (idx_t *)realloc(phi, (n + 1) * sizeof(idx_t));
    if (!phi)
        abort();

    A = (seq_t *)realloc(A, (n + 1) * sizeof(seq_t));
    if (!A)
        abort();

    /* Compute Euler's totient function for all 1..n */
    for (idx_t i = 1; i <= n; ++i)
        phi[i] = i;
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == p) {               /* p is prime */
            for (idx_t k = p; k <= n; k += p) {
                phi[k] -= phi[k] / p;
            }
        }
    }

    /* Compute A[k] = sum_{d|k} d * phi(k/d) */
    for (idx_t i = 0; i <= n; ++i)
        A[i] = 0;
    for (idx_t d = 1; d <= n; ++d) {
        for (idx_t t = 1; d * t <= n; ++t) {
            idx_t m = d * t;
            seq_t inc = (seq_t)d * (seq_t)phi[t];
            A[m] += inc;
        }
    }

    limit = n;
}

/* Public function to compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    recompute_up_to(n);
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd((seq_t)k, n) == 1)
            sum += A[k];
    }
    return sum;
}
