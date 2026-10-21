/*
 * A202153  G.f.: Sum_{n>=0} x^n * Product_{k=0..n-1} (1 + (2*k+1)*x).
 * https://oeis.org/A202153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in the series * G(x) = Σ_{m≥0}
 *            x^m ∏_{k=0}^{m-1} (1 + (2k+1)x) * * This implementation computes
 *            a(n) incrementally, storing all * contributions from each
 *            intermediate polynomial P_m. * Complexity: O(n^2) arithmetic
 *            operations, with O(n) memory
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 30, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed values */
static seq_t *a_cache = NULL;            /* a_cache[i] = a(i) */
static size_t a_cap  = 0;                /* allocated size for a_cache */
static idx_t  current_m = 0;             /* maximum m that has been processed */

/* Cache for the current polynomial P_m(x) */
static seq_t *poly = NULL;               /* poly[k] = coefficient of x^k in P_m */
static size_t poly_cap = 0;              /* allocated size for poly */

/* Ensure a_cache has space for computed indices up to 'needed' (inclusive). */
static void ensure_a_capacity(size_t needed)
{
    if (needed < a_cap) return;
    size_t new_cap = a_cap ? a_cap << 1 : 128;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();               /* out of memory */
    for (size_t i = a_cap; i < new_cap; ++i) new_arr[i] = 0;
    a_cache = new_arr;
    a_cap = new_cap;
}

/* Ensure poly has space for indices up to 'needed' (inclusive). */
static void ensure_poly_capacity(size_t needed)
{
    if (needed < poly_cap) return;
    size_t new_cap = poly_cap ? poly_cap << 1 : 128;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(poly, new_cap * sizeof(seq_t));
    if (!new_arr) abort();               /* out of memory */
    for (size_t i = poly_cap; i < new_cap; ++i) new_arr[i] = 0;
    poly = new_arr;
    poly_cap = new_cap;
}

/* Initialise the caches if they are empty (m = 0). */
static void init_if_needed(void)
{
    if (a_cache || poly) return;  /* already initialised */

    /* initialise P_0(x) = 1 */
    ensure_poly_capacity(1);
    poly[0] = (seq_t)1;

    /* initialise a[0] */
    ensure_a_capacity(0);
    a_cache[0] = (seq_t)0;
    a_cache[0] += poly[0];           /* a(0) = 1 */

    current_m = 0;
}

/* Compute all values up to and including a(n).
 * After this call, a_cache[n] is valid. */
static void compute_until(idx_t n)
{
    init_if_needed();
    if (n <= current_m) return;      /* nothing new to compute */

    for (idx_t m = current_m + 1; m <= n; ++m) {
        /* Make sure poly has room for coefficient 'm' */
        ensure_poly_capacity((size_t)m + 1);

        /* New coefficient poly[m] starts at 0 */
        poly[m] = (seq_t)0;

        /* Multiply the previous polynomial by (1 + (2m-1)x). */
        seq_t factor = (seq_t)m; /* factor = m */
        factor <<= 1;            /* factor = 2*m */
        factor -= 1;             /* factor = 2*m - 1 */

        for (idx_t k = m; k > 0; --k) {
            poly[k] = poly[k] + factor * poly[k - 1];
        }
        /* poly[0] remains 1 */

        /* Add contributions to a_cache for indices n = m + k */
        for (idx_t k = 0; k <= m; ++k) {
            size_t pos = (size_t)m + (size_t)k;   /* n = m + k */
            ensure_a_capacity(pos);
            a_cache[pos] += poly[k];
        }
    }
    current_m = n;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    compute_until(n);
    return a_cache[n];
}
