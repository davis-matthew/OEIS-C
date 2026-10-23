/*
 * A034775  Dirichlet convolution of d(n) (# of divisors) with Bell numbers.
 * https://oeis.org/A034775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} d(d) * B(n/d - 1) where d(k) is the number of
 *            divisors of k and B(m) is the m‑th Bell number (B(0)=1)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/binom.h"

static seq_t *Bell      = NULL;    /* Bell numbers B[0..N] */
static seq_t *divcnt    = NULL;    /* divisor counts d[0..N] */
static size_t cached_n  = 0;        /* largest n for which we have cached data */

/* Ensure precomputed data up to at least n. */
static void ensure_precomputed(idx_t n)
{
    if ((size_t)n <= cached_n)   /* already computed up to this n */
        return;

    /* Reallocate arrays to size n+1 (indices 0..n). */
    if (Bell)      seq_free_array(Bell);
    if (divcnt)    seq_free_array(divcnt);
    Bell    = seq_alloc_new_array((size_t)n + 1);
    divcnt  = seq_alloc_new_array((size_t)n + 1);

    /* Initialize to zero. */
    size_t i;
    for (i = 0; i <= (size_t)n; ++i) {
        Bell[i]   = 0;
        divcnt[i] = 0;
    }

    /* Divisor counting via sieve. */
    for (i = 1; i <= (size_t)n; ++i)
        for (size_t j = i; j <= (size_t)n; j += i)
            divcnt[j] += 1;

    /* Bell numbers: B[0] = 1; B[m+1] = Σ_{k=0..m} C(m,k) * B[k] */
    Bell[0] = 1;
    for (size_t m = 0; m < (size_t)n; ++m) {
        seq_t sum = 0;
        for (size_t k = 0; k <= m; ++k) {
            seq_t c = binom((idx_t)m, (idx_t)k);
            sum += c * Bell[k];
        }
        Bell[m + 1] = sum;
    }

    cached_n = (size_t)n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                /* sequence starts at offset 1 */
        return 0;

    ensure_precomputed(n);

    seq_t total = 0;

    /* Sum over divisors d of n. */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* term for divisor d1: d(d1) * B(n/d1 - 1) */
            size_t idx1 = (size_t)(n / d1 - 1);
            seq_t term1 = divcnt[d1] * Bell[idx1];
            total += term1;

            if (d1 != d2) {      /* term for divisor d2 if distinct */
                size_t idx2 = (size_t)(n / d2 - 1);
                seq_t term2 = divcnt[d2] * Bell[idx2];
                total += term2;
            }
        }
    }
    return total;
}
