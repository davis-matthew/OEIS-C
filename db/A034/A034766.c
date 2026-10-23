/*
 * A034766  Dirichlet convolution of phi(n) with Catalan numbers.
 * https://oeis.org/A034766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * Catalan(n/d) , where Catalan(k) =
 *            C_{k-1} (standard Catalan sequence offset 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cached values */
static seq_t *phi_cache = NULL;   /* phi_cache[i] = φ(i) for 0 <= i <= cache_size */
static seq_t *cat_cache = NULL;   /* cat_cache[k]   = Catalan(k) with offset 1 (k>=1) */
static idx_t cache_size = 0;

/* Build or extend the caches up to at least n (inclusive). */
static void init_caches(idx_t n)
{
    if (cache_size >= n)
        return;                      /* already prepared */

    /* Allocate the arrays.  cat_cache needs an extra slot because we index up to n. */
    phi_cache = (seq_t *)realloc(phi_cache, ((size_t)n + 1) * sizeof(seq_t));
    cat_cache = (seq_t *)realloc(cat_cache, ((size_t)n + 2) * sizeof(seq_t));

    /* --- Compute Euler’s totient φ(i) for 1 <= i <= n --- */
    for (idx_t i = 0; i <= n; ++i)
        phi_cache[i] = (seq_t)i;     /* start with φ(i)=i */
    for (idx_t i = 2; i <= n; ++i) {
        if (phi_cache[i] == (seq_t)i) {          /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                phi_cache[j] -= phi_cache[j] / (seq_t)i;
        }
    }

    /* --- Compute Catalan numbers with offset 1:
          cat[k] = C_{k-1}  for k >= 1   (so cat[1]=1, cat[2]=1, ...)  */
    if (n >= 1)
        cat_cache[1] = (seq_t)1;   /* C0 */
    if (n >= 2)
        cat_cache[2] = (seq_t)1;   /* C1 */

    /* Use recurrence: C_{m+1} = C_m * (4m+2)/(m+2) for standard C_m.
       For our offset, cat[k+1] = cat[k] * (4k-2)/(k+1).            */
    for (idx_t k = 2; k <= n - 1; ++k) {
        seq_t numer = cat_cache[k] * ((seq_t)(4 * k - 2));
        seq_t denom = (seq_t)(k + 1);
        cat_cache[k + 1] = numer / denom;           /* exact division */
    }

    cache_size = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined below the offset */

    init_caches(n);

    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sum += phi_cache[d] * cat_cache[n / d];
            idx_t d2 = n / d;
            if (d2 != d)
                sum += phi_cache[d2] * cat_cache[d];
        }
    }
    return sum;
}
