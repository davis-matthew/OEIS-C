/*
 * A049126  Revert transform of ((x - 1)(3x - 1))/(1 - 3x + x^3).
 * https://oeis.org/A049126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049126: Revert transform of ((x - 1)(3x - 1))/(1 - 3x + x^3).
   Recurrence (offset 1):
   12*(n-1)*n*(117*n - 604)*a(n) =
          4*(n-1)*(4329*n^2 - 29719*n + 39384)*a(n-1)
        - 3*(23985*n^3 - 230056*n^2 + 679659*n - 630308)*a(n-2)
        + 3*(38961*n^3 - 437004*n^2 + 1609875*n - 1955812)*a(n-3)
        - 27*(n-4)*(2223*n^2 - 18496*n + 38133)*a(n-4)
        + 81*(n-5)*(n-4)*(117*n - 487)*a(n-5).
   Initial terms (offset 1): 1, 1, 2, 6, 22. */
#include <stdlib.h>   /* malloc, realloc, free, size_t */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>  /* bool */

/* BigT is used for intermediate arithmetic.
   If the compiler supports unsigned __int128 and seq_t is a
   built‑in integer type smaller than 128 bits, we use it to avoid
   overflow when computing the recurrence.  For arbitrary‑precision
   seq_t we simply alias BigT to seq_t. */
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
  typedef unsigned __int128 BigT;
#else
  typedef seq_t BigT;
#endif

/* Cached sequence values: cache[i] holds a(i+1), 0‑based.
   cache_used records the largest n stored (>=0). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_used = 0;

/* Ensure cache has capacity at least 'need'. */
static void
ensure_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (cache == NULL)
        abort();        /* allocation failed */
    cache_cap = new_cap;
}

/* Initialise first five terms (offset 1). */
static void
init_seed(void)
{
    if (cache_used >= 5)
        return;
    ensure_cap(5);
    cache[0] = (seq_t)1;   /* a(1) */
    cache[1] = (seq_t)1;   /* a(2) */
    cache[2] = (seq_t)2;   /* a(3) */
    cache[3] = (seq_t)6;   /* a(4) */
    cache[4] = (seq_t)22;  /* a(5) */
    cache_used = 5;
}

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;   /* undefined for n < 1 */

    init_seed();
    ensure_cap((size_t)n);

    /* Compute missing terms up to n. */
    for (idx_t k = (idx_t)(cache_used + 1); k <= n; ++k) {
        /* k is at least 6 after init_seed */
        BigT nb = (BigT)k;
        BigT n1 = nb - (BigT)1;
        BigT n2 = nb * nb;
        BigT n3 = n2 * nb;

        /* Coefficients for each term */
        BigT c1 = (BigT)4 * n1 *
                  ((BigT)4329 * n2 - (BigT)29719 * nb + (BigT)39384);
        BigT c2 = (BigT)3 *
                  ((BigT)23985 * n3 - (BigT)230056 * n2 + (BigT)679659 * nb
                   - (BigT)630308);
        BigT c3 = (BigT)3 *
                  ((BigT)38961 * n3 - (BigT)437004 * n2 + (BigT)1609875 * nb
                   - (BigT)1955812);
        BigT c4 = (BigT)27 * (nb - (BigT)4) *
                  ((BigT)2223 * n2 - (BigT)18496 * nb + (BigT)38133);
        BigT c5 = (BigT)81 * (nb - (BigT)5) * (nb - (BigT)4) *
                  ((BigT)117 * nb - (BigT)487);

        /* Previous terms (ensure cast to BigT) */
        BigT a1 = (BigT)cache[(size_t)(k - 2)];   /* a(k-1) */
        BigT a2 = (BigT)cache[(size_t)(k - 3)];   /* a(k-2) */
        BigT a3 = (BigT)cache[(size_t)(k - 4)];   /* a(k-3) */
        BigT a4 = (BigT)cache[(size_t)(k - 5)];   /* a(k-4) */
        BigT a5 = (BigT)cache[(size_t)(k - 6)];   /* a(k-5) */

        /* Numerator of the recurrence */
        BigT numer = c1 * a1 - c2 * a2 + c3 * a3 - c4 * a4 + c5 * a5;

        /* Denominator of the recurrence */
        BigT denom = (BigT)12 * n1 * nb * ((BigT)117 * nb - (BigT)604);

        /* Exact division */
        BigT ak = numer / denom;

        cache[(size_t)(k - 1)] = (seq_t)ak;
    }

    cache_used = (size_t)n;
    return cache[(size_t)(n - 1)];
}
