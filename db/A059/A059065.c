/*
 * A059065  Card-matching numbers (Dinner-Diner matching numbers).
 * https://oeis.org/A059065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, nice
 * Formula:   Let k = ⌊√n⌋. Then p = n – k² satisfies 0 ≤ p ≤ 2k. If p is odd
 *            the entry is 0. If p is even, set m = p/2. Then a(n) = (k! ·
 *            C(k, m))² where C(k, m) = k! / (m! · (k–m)!) is the binomial
 *            coefficient. This produces the row‑by‑row reading of the
 *            card‑matching triangle for two kinds of cards. The seq...
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---- helper: integer square root for 64‑bit unsigned ---- */
static idx_t uint64_sqrt(idx_t n) {
    idx_t low = 0, high = 0xFFFFFFFFULL; /* sqrt(2^64-1) ≈ 4.3×10^9 */
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n)      return mid;
        if (sq < n)       low  = mid + 1;
        else              high = mid - 1;
    }
    return high;          /* floor(sqrt(n)) */
}

/* ---- factorial cache ------------------------------------ */
static seq_t *fact_cache = NULL;
static idx_t fact_size  = 0;

/* grow cache to at least upto */
static void ensure_fact(idx_t upto) {
    if (upto < fact_size) return;
    size_t new_size = upto + 1;
    seq_t *new_cache = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
    if (!new_cache) { /* allocation failed – abort gracefully */
        /* In an extreme failure we return 0 for all future values. */
        fact_cache = NULL;
        fact_size  = 0;
        return;
    }
    /* initialise new entries */
    if (fact_size == 0) {
        new_cache[0] = 1;      /* 0! = 1 */
        fact_size = 1;
    }
    for (idx_t i = fact_size; i <= upto; ++i) {
        new_cache[i] = new_cache[i-1] * (seq_t)i;
    }
    fact_cache = new_cache;
    fact_size  = new_size;
}

/* ---- main function ------------------------------------- */
seq_t a(idx_t n) {
    idx_t k = uint64_sqrt(n);          /* floor(sqrt(n)) */
    idx_t j = n - k*k;                 /* position within row k (0 … 2k) */

    /* odd positions give zero */
    if (j & 1U) return 0;

    idx_t m = j >> 1;                  /* j/2 */

    /* r = k!  (cache will be expanded as needed) */
    ensure_fact(k);
    if (!fact_cache) return 0;         /* allocation failure */

    seq_t fact_k      = fact_cache[k];
    seq_t fact_m      = fact_cache[m];
    seq_t fact_k_m    = fact_cache[k - m];

    /* C(k, m) = k! / (m! (k-m)!) */
    seq_t denom = fact_m * fact_k_m;
    seq_t binom = fact_k / denom;      /* exact division */

    /* value = (k! * C(k, m))² */
    seq_t val = fact_k * binom;
    return val * val;
}
