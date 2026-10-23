/*
 * A169406  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A169406:
 *   a(n) = 7*(a(n-1)+...+a(n-31)) - 28*a(n-32)   for n >= 32
 * Initial values a(0)..a(31) are obtained from the power-series
 * expansion of (1+2*∑_{k=1}^{31}t^k+t^32)/(1-7*∑_{k=1}^{31}t^k+28t^32). */
/* --- Implementation follows --- */

#include <stdlib.h>

static seq_t *cache = NULL;    /* dynamic array holding computed terms */
static size_t cache_cap = 0;   /* allocated capacity (in elements) */
static idx_t  cache_len = 0;   /* number of terms actually stored */

/* Ensure that cache has space for indices up to 'n' (inclusive) */
static void ensure_cap(idx_t n) {
    if (n < (idx_t)cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 32;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* out‑of‑memory is fatal for this simple lib */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the first 32 terms a(0)..a(31) from the defining power series. */
static void init_base(void) {
    if (cache_len > 0)   /* already initialized */
        return;

    ensure_cap(31);
    /* temporary array for the first 32 terms */
    seq_t base[32];
    for (idx_t n = 0; n <= 31; ++n) {
        seq_t sumprev = 0;                 /* Σ_{k=1..n} a(n-k) */
        for (idx_t k = 1; k <= n; ++k)
            sumprev += base[n - k];
        /* N[n] = 1 for n=0, otherwise 2 for 1≤n≤31 */
        seq_t Nn = (n == 0) ? 1 : 2;
        base[n] = Nn + 7 * sumprev;        /* a[n] = Nn + 7*Σ a[n-k] */
    }
    /* copy into cache */
    for (idx_t i = 0; i <= 31; ++i)
        cache[i] = base[i];
    cache_len = 32;
}

/* Main interface */
seq_t a(idx_t n) {
    if (n == (idx_t)-1)  /* defensive: avoid overflow if idx_t is unsigned */
        return 0;

    if ((idx_t)n >= cache_len)
        init_base();   /* first time: initialise base terms */

    ensure_cap(n);
    /* compute any missing terms up to 'n' */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 31) {
            /* base terms already present */
            continue;
        }
        /* recurrence: a[i] = 7*(a[i-1]+...+a[i-31]) - 28*a[i-32] */
        seq_t sumprev = 0;
        for (idx_t k = 1; k <= 31; ++k)
            sumprev += cache[i - k];
        seq_t term = 7 * sumprev;
        term -= 28 * cache[i - 32];
        cache[i] = term;
    }
    cache_len = (n + 1 > cache_len) ? n + 1 : cache_len;  /* update length */
    return cache[n];
}
