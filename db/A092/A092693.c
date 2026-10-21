/*
 * A092693  Sum of iterated phi(n).
 * https://oeis.org/A092693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) + a(phi(n)), with a(1) = 0
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of iterated Euler totients. */

#include <stdlib.h>

/* Static caches for φ and a(n). They are rebuilt from scratch
   whenever an n larger than the current cache maximum is requested. */
static seq_t *phi_cache = NULL;
static seq_t *a_cache   = NULL;
static idx_t  cache_max = 0;        /* largest n for which caches are valid */

/* Allocate and compute caches up to at least n. */
static void ensure_upto(idx_t n)
{
    if (n <= cache_max) return;           /* already computed */

    /* allocate new buffers of size n+1 (index 0 unused for simplicity) */
    size_t new_sz = (size_t)(n + 1);
    seq_t *new_phi;
    seq_t *new_a;

#ifdef __cplusplus
    new_phi = new seq_t[new_sz];
    new_a   = new seq_t[new_sz];
#else
    new_phi = (seq_t*)malloc(new_sz * sizeof(seq_t));
    new_a   = (seq_t*)malloc(new_sz * sizeof(seq_t));
    if (!new_phi || !new_a) abort();  /* allocation failure */
#endif

    /* initialise φ[i] = i for all i */
    for (idx_t i = 0; i < new_sz; ++i) new_phi[i] = (seq_t)i;
    if (new_sz > 0) new_phi[0] = 0;  /* not used */

    /* sieve to compute totients for 2 .. n */
    for (idx_t p = 2; p <= n; ++p)
    {
        if (new_phi[p] == (seq_t)p)        /* p is prime */
        {
            seq_t sp = (seq_t)p;
            seq_t sp1 = sp - 1;
            for (idx_t j = p; j <= n; j += p)
                new_phi[j] = new_phi[j] / sp * sp1;
        }
    }

    /* compute a(i) for i up to n (φ[i] < i for i > 1) */
    if (new_sz > 0) new_a[0] = 0;      /* unused */
    if (new_sz > 1) new_a[1] = 0;      /* a(1) = 0 */
    for (idx_t i = 2; i <= n; ++i)
    {
        idx_t phi_i = (idx_t)new_phi[i];   /* φ(i) fits in idx_t (≤ i) */
        new_a[i] = new_phi[i] + new_a[phi_i];
    }

    /* free old caches */
#ifdef __cplusplus
    delete[] phi_cache;
    delete[] a_cache;
#else
    free(phi_cache);
    free(a_cache);
#endif

    phi_cache = new_phi;
    a_cache   = new_a;
    cache_max = n;
}

/* Required external function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined indices return 0 */
    ensure_upto(n);
    return a_cache[n];
}
