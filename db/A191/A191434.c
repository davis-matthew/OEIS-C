/*
 * A191434  Dispersion of ([n*x+n+3/2]), where x=(golden ratio) and [ ]=floor,
 *          by antidiagonals.
 * https://oeis.org/A191434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = element at position n obtained by flattening the *
 *            dispersion array of the Beatty sequence s(k) = ⌊k*φ² + 3/2⌋ * (φ
 *            = (1+√5)/2) on antidiagonals
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Compute s(k) = ⌊k*φ² + 3/2⌋ exactly for any seq_t k.  We avoid
 * floating‑point arithmetic by using the identity
 *
 *   φ² = (3 + √5)/2
 *   s(k) = ⌊(3k + k√5 + 3)/2⌋
 *
 * Let a = ⌊k√5⌋ = floor(√(5k²)).  Then
 *   s(k) = ⌊(a + 3k + 3)/2⌋.
 * The parity of (a + 3k + 3) determines whether we subtract 1 before
 * dividing by 2. */
static seq_t s_value(seq_t k)
{
    seq_t k_sq = k * k;                     /* k²   */
    seq_t five_k_sq = (seq_t)5 * k_sq;       /* 5k²  */
    seq_t a = floor_sqrt(five_k_sq);         /* ⌊k√5⌋  */
    seq_t c = (seq_t)3 * k + (seq_t)3;       /* 3k+3  */
    seq_t sum = a + c;                       /* a+c   */

    /* If sum is even, division by 2 gives the floor; else subtract 1. */
    if ((sum & (seq_t)1) == 0)
        return sum / (seq_t)2;
    else
        return (sum - (seq_t)1) / (seq_t)2;
}

/* ----------  Cache for the complement t(n) of s(n)  ---------- */

static seq_t *t_cache = NULL;      /* array of t(1), t(2), … */
static size_t t_cache_count = 0;   /* number of filled entries */
static size_t t_cache_cap = 0;     /* allocated capacity */

/* state while generating t values sequentially */
static idx_t t_next_n = 1;         /* current natural number being examined */
static idx_t t_next_k = 1;         /* current index into s() */
static seq_t t_next_s = 0;         /* s(t_next_k) */

/* Initialise the cache if it does not yet exist.  */
static void t_cache_init(void)
{
    if (!t_cache) {
        size_t cap = 16;
        t_cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!t_cache) abort();
        t_cache_cap  = cap;
        t_cache_count = 0;
        t_next_n = 1;
        t_next_k = 1;
        t_next_s = s_value((seq_t)1);
    }
}

/* Ensure that t_cache contains at least needed entries. */
static void ensure_t_cache(size_t needed)
{
    if (!t_cache) t_cache_init();
    /* Grow capacity if necessary.  */
    while (t_cache_cap < needed) {
        size_t new_cap = t_cache_cap * 2;
        if (new_cap < 16) new_cap = 16;
        seq_t *new_arr = (seq_t *)realloc(t_cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        t_cache = new_arr;
        t_cache_cap = new_cap;
    }

    /* Generate more entries until we have the required amount.  */
    while (t_cache_count < needed) {
        if ((idx_t)t_next_s == t_next_n) {
            /* n belongs to s, skip it.  */
            ++t_next_k;
            t_next_s = s_value((seq_t)t_next_k);
            ++t_next_n;
        } else {
            /* n is a t value.  */
            t_cache[t_cache_count++] = t_next_n;
            ++t_next_n;
        }
    }
}

/* ----------  Main sequence function  ---------- */

/* Return a(n) (1‑based).  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1 */

    /* Find the antidiagonal (sum i+j) that contains a(n).  */
    idx_t diag = 2;  /* minimal i+j */
    while ((diag - 1) * diag / 2 < n)
        ++diag;

    /* Number of terms before this diagonal.  */
    idx_t prev_terms = (diag - 2) * (diag - 1) / 2;
    idx_t offset = n - prev_terms - 1;   /* 0‑based position on the diag. */

    idx_t i = offset + 1;    /* row index (1‑based) */
    idx_t j = diag - i;      /* column index (≥1) */

    /* Ensure the needed t(i) value is cached.  */
    ensure_t_cache((size_t)i);
    seq_t val = t_cache[i - 1];

    /* Apply s (j-1) times.  */
    for (idx_t step = 1; step < j; ++step)
        val = s_value(val);

    return val;
}
