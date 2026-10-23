/*
 * A049924  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 3, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1 and a(2) = 3.
 * https://oeis.org/A049924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 3; for n ≥ 3, a(n) = a(1)+...+a(n-1) – a(m)
 *            where m = 2*n – 3 – 2^(p+1) and p is the unique integer with 2^p
 *            < n – 1 ≤ 2^(p+1)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;          /* a(n) values, 1‑based index  */
static seq_t *S_cache = NULL;          /* prefix sums S(n) = a(1)+···+a(n) */
static idx_t cache_cap = 0;            /* size of cached arrays (max n computed) */

/* Return floor(log2(v)) for v > 0.  This is used to find p. */
static unsigned
floor_log2_u64(idx_t v)
{
    unsigned r = 0;
    while (v >>= 1)
        ++r;
    return r;
}

/* Ensure the cache contains the sequence up to index n */
static void
ensure_cache(idx_t n)
{
    if (cache_cap >= n)                /* already computed */
        return;

    /* Allocate new arrays (indices 0..n, we use 1..n) */
    idx_t cap = n + 1;   /* capacity needed */
    seq_t *a_new = (seq_t *)malloc(sizeof(seq_t) * (size_t)cap);
    seq_t *S_new = (seq_t *)malloc(sizeof(seq_t) * (size_t)cap);
    if (!a_new || !S_new) {
        /* allocation failure – leave cache unchanged to avoid corruption */
        free(a_new);
        free(S_new);
        return;
    }

    /* Initialize zero entry for convenience */
    a_new[0] = 0;
    S_new[0] = 0;

    /* Copy any previously cached values */
    for (idx_t i = 1; i <= cache_cap; ++i) {
        a_new[i] = a_cache[i];
        S_new[i] = S_cache[i];
    }

    /* Compute new entries sequentially */
    for (idx_t k = (cache_cap > 0 ? cache_cap + 1 : 1); k <= n; ++k) {
        if (k == 1) {
            a_new[1] = (seq_t)1;
            S_new[1] = a_new[1];
        } else if (k == 2) {
            a_new[2] = (seq_t)3;
            S_new[2] = S_new[1] + a_new[2];
        } else {
            idx_t t = k - 1;      /* n – 1 */
            unsigned p = floor_log2_u64(t - 1);   /* 2^p < t ≤ 2^(p+1)  */
            idx_t m = 2 * k - 3 - ((idx_t)1 << (p + 1));
            a_new[k] = S_new[k - 1] - a_new[m];
            S_new[k] = S_new[k - 1] + a_new[k];
        }
    }

    /* Free old arrays */
    free(a_cache);
    free(S_cache);

    /* Assign new arrays and update cache size */
    a_cache = a_new;
    S_cache = S_new;
    cache_cap = n;
}

/* Public function: returns a(n) for any n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* sequence is not defined for n = 0 */
    ensure_cache(n);           /* compute up to n if needed */
    return a_cache[n];
}
