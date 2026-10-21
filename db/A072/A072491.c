/*
 * A072491  Define f(1) = 0. For n>=2, let f(n) = n - p where p is the largest
 *          prime <= n. a(n) = number of iterations of f to reach 0, starting
 *          from n.
 * https://oeis.org/A072491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of iterations of f(k)=k‑largest_prime_leq(k) *
 *            until reaching 0 (a(0)=0, a(1)=1). The function uses a * dynamic
 *            cache that grows as needed
 * Range:     u8 n <= 9539, u16 n <= 9539, u32 n <= 9539, u64 n <= 9539,
 *            u128 n <= 9539, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static seq_t *a_cache   = NULL;   /* memoised a(n) values                      */
static idx_t *lp_cache  = NULL;   /* largest prime ≤ i for each i up to cur_cap-1 */
static idx_t cur_cap    = 0;      /* current size of the two caches            */

/* Ensure that the cache holds values up to at least n.  The routine
 * doubles the capacity until it is sufficient, then rebuilds the prime
 * sieve and recomputes all a(i) up to the new capacity.
 */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cur_cap)
        return;   /* already enough */

    /* Determine new capacity (power of two at least n+1).  We add 1
     * afterwards to get the number of slots (indices 0..new_cap-1).
     */
    idx_t new_cap = cur_cap > 0 ? cur_cap << 1 : 2;
    while (new_cap <= n)
        new_cap <<= 1;
    new_cap += 1;                    /* number of slots needed          */

    /* Allocate space for the new arrays.  We use 'new' when compiling
     * C++ (so that default constructors are run for non‑trivial seq_t)
     * and malloc otherwise.
     */
    seq_t *new_a;
    idx_t *new_lp;
#ifdef __cplusplus
    new_a  = new seq_t[new_cap];
    new_lp = new idx_t[new_cap];
#else
    new_a  = (seq_t *)malloc(new_cap * sizeof(seq_t));
    new_lp = (idx_t *)malloc(new_cap * sizeof(idx_t));
#endif
    if (!new_a || !new_lp)
        abort();

    /* Create a simple sieve of Eratosthenes up to new_cap‑1.  We only
     * need to know whether a number is composite; a byte per entry
     * suffices.
     */
    uint8_t *comp = (uint8_t *)calloc(new_cap, 1);  /* 0 = prime, 1 = composite */
    if (!comp)
        abort();

    if (new_cap > 1) { comp[0] = comp[1] = 1; }
    for (idx_t i = 2; i * i < new_cap; ++i) {
        if (!comp[i]) {
            for (idx_t j = i * i; j < new_cap; j += i)
                comp[j] = 1;
        }
    }

    /* Build the largest‑prime array while simultaneously computing
     * a(i).  For i < 2, the largest prime ≤ i is 0.
     */
    idx_t last_prime = 0;
    new_a[0] = (seq_t)0;
    if (new_cap > 1)
        new_a[1] = (seq_t)1;

    for (idx_t i = 0; i < new_cap; ++i) {
        if (i >= 2 && !comp[i])          /* i is prime */
            last_prime = i;
        new_lp[i] = last_prime;
    }

    for (idx_t i = 2; i < new_cap; ++i) {
        idx_t prev = i - new_lp[i];
        new_a[i] = new_a[prev] + (seq_t)1;
    }

    /* Free old caches if they exist.  Use delete[] in C++ and free in C. */
#ifdef __cplusplus
    delete[] a_cache;
    delete[] lp_cache;
#else
    free(a_cache);
    free(lp_cache);
#endif
    free(comp);

    /* Adopt the freshly computed arrays. */
    a_cache  = new_a;
    lp_cache = new_lp;
    cur_cap  = new_cap;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    return a_cache[n];
}
