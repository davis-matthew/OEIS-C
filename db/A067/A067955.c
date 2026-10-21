/*
 * A067955  Number of dissections of a convex polygon by nonintersecting
 *          diagonals into polygons with even number of sides and having a
 *          total number of n edges (sides and diagonals).
 * https://oeis.org/A067955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the cubic equation of its generating function
 *            (1+z)G(z)^3 – zG(z)^2 – G(z) + z = 0, where G(z)=∑_{k≥1}a(k)z^k.
 *            From this equation one obtains the recurrence a(n) = c_n +
 *            c_{n‑1} – C2[n‑1] (n ≥ 2) with C2[m] = Σ_{i=1}^{m‑1} a(i)·a(m‑i)
 *            (convolution of a with itself) c_m = Σ_{i=1...
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 37, u64 n <= 65, u128 n <= 122,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include <stdbool.h>         /* for bool (not used but required by the prelude) */
#include "../utils/seq_realloc.h"  /* provides seq_realloc */

static seq_t *a_arr = NULL;          /* a[n]  (1‑based) */
static seq_t *c2_arr = NULL;         /* C2[n] (1‑based) */
static size_t cap = 0;               /* currently allocated capacity (≥ n+1) */
static idx_t  max_computed = 0;      /* largest n already computed */
static seq_t  prev_c = 0;            /* c_{max_computed} */

/* Increase the capacity of the two static arrays to hold indices up to n. */
static void ensure_capacity(idx_t n)
{
    idx_t need = n + 1;            /* because we use 1‑based indices */
    if ((size_t)need <= cap) return;

    size_t new_cap = (size_t)need;
    a_arr  = seq_realloc(a_arr,  cap, new_cap);
    c2_arr = seq_realloc(c2_arr, cap, new_cap);
    /* initialise newly allocated elements to zero */
    for (size_t i = cap; i < new_cap; ++i) {
        a_arr[i]  = 0;
        c2_arr[i] = 0;
    }
    cap = new_cap;
}

/* Public function: a(n) as defined above. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    if (max_computed == 0) {
        /* initialise base case */
        ensure_capacity(1);          /* need indices 0 and 1 */
        a_arr[1]  = 1;               /* a(1) = 1 */
        c2_arr[1] = 0;               /* C2[1] = 0 */
        prev_c    = 0;               /* c_1 = 0 */
        max_computed = 1;
    }

    if (n <= max_computed) return a_arr[n];

    ensure_capacity(n);

    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* compute C2[k‑1] = Σ_{i=1}^{k‑2} a[i]*a[k-1-i] */
        seq_t sum_C2 = 0;
        for (idx_t i = 1; i <= k - 2; ++i)
            sum_C2 += a_arr[i] * a_arr[(k - 1) - i];
        c2_arr[k - 1] = sum_C2;

        /* compute c_k = Σ_{i=1}^{k‑2} a[i]*C2[k-i] */
        seq_t c_k = 0;
        for (idx_t i = 1; i <= k - 2; ++i)
            c_k += a_arr[i] * c2_arr[k - i];

        /* compute a_k = c_k + prev_c - sum_C2 */
        a_arr[k] = c_k + prev_c - sum_C2;

        /* update prev_c for next iteration */
        prev_c = c_k;
    }

    max_computed = n;
    return a_arr[n];
}
