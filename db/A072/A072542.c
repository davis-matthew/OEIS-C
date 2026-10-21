/*
 * A072542  a(n) = the least positive integer k such that sigma(k) > sigma(1)
 *          + ... + sigma(n).
 * https://oeis.org/A072542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer k such that sigma(k) > sigma(1) +
 *            sigma(2) + … + sigma(n) (OEIS A072542)
 * Range:     u8 n <= 17, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Build will supply these types */
extern seq_t a(idx_t n);

/* cached divisor sums and their prefix sums (1‑based indices) */
static seq_t *sigma = NULL;   /* sigma[i] = σ(i) */
static seq_t *pref  = NULL;   /* pref[i]  = Σ_{j=1..i} σ(j) */
static size_t max_j = 0;      /* highest index currently computed */

/* Compute or extend the tables up to at least 'limit' (inclusive) */
static void extend_to(size_t limit)
{
    if (limit <= max_j) return;

    /* enlarge the arrays */
    seq_t *new_sigma = (seq_t*)calloc(limit + 1, sizeof(seq_t));
    seq_t *new_pref  = (seq_t*)calloc(limit + 1, sizeof(seq_t));

    if (max_j > 0) {
        memcpy(new_sigma, sigma, (max_j + 1) * sizeof(seq_t));
        memcpy(new_pref,  pref , (max_j + 1) * sizeof(seq_t));
    }

    /* Add divisor contributions for indices > max_j */
    size_t old = max_j;
    for (size_t d = 1; d <= limit; ++d) {
        /* first multiple of d that is > old  */
        size_t start = (old / d + 1) * d;
        if (start < d) start = d;          /* for old==0 */
        for (size_t j = start; j <= limit; j += d) {
            new_sigma[j] += (seq_t)d;
        }
    }

    /* Recompute prefix sums (re‑computing is cheap for the needed sizes) */
    seq_t acc = 0;
    for (size_t i = 1; i <= limit; ++i) {
        acc += new_sigma[i];
        new_pref[i] = acc;
    }

    /* replace old tables */
    free(sigma);
    free(pref);
    sigma = new_sigma;
    pref  = new_pref;
    max_j = limit;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used, guard */

    size_t N = (size_t)n;

    /* Ensure divisor sums up to at least n are known */
    extend_to(N);

    /* Target sum S = Σ_{i=1..n} σ(i) */
    seq_t target = pref[N];

    /* Rough upper bound for the search – n*10 is safe for n≤10000 */
    size_t approx = N * 10;
    if (approx < max_j) approx = max_j;
    extend_to(approx);

    /* Find the smallest k with σ(k) > target */
    for (size_t k = 1;; ++k) {
        if (k > max_j) extend_to(k);   /* safety, usually not needed */
        if (sigma[k] > target) return (seq_t)k;
    }
}
