/*
 * A126933  Quotients arising from sequence A053312.
 * https://oeis.org/A126933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (a(n-1) + d*5^{n-1}) / 2 with d = 1 if a(n-1) odd, else
 *            2. * Initial value: a(1) = 1. The recurrence was derived from
 *            the divisibility * condition V_n = 10^{n-1}d_n + ... + 10^0d_1 ≡
 *            0 (mod 2^n). The binary * parity rule yields the choice of digit
 *            d. The integer * a(n) = V_n / 2^n is store...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised cache of the sequence terms */
static seq_t *cache      = NULL;   /* cache[n-1] holds a(n)          */
static size_t cache_cap  = 0;      /* currently allocated capacity   */
static size_t cache_len  = 0;      /* number of terms stored         */

/* The power of 5 used for the next update:
 * pow5_val = 5^(current_len).  After computing a(current_len+1),
 * pow5_val will be multiplied by 5 for the next step.           */
static seq_t pow5_val = 0;

/* Make sure the cache can hold at least `need` terms. */
static void
ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();               /* allocation failure is fatal */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_arr[i] = cache[i];
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* defined only for n ≥ 1, but safe */

    /* Initialise cache with first term if necessary. */
    if (cache_len == 0) {
        ensure_cache(1);
        cache[0] = 1;                   /* a(1) = 1 */
        cache_len = 1;
        cache_cap = 1;
        pow5_val = 5;                   /* 5^1 for next step (m = 1) */
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len) return cache[n-1];

    /* Grow cache and compute terms up to n. */
    ensure_cache((size_t)n);

    /* Iteratively compute new values using the recurrence. */
    for (idx_t m = cache_len; m < n; ++m) {
        seq_t prev = cache[m-1];
        seq_t d = (prev & 1) ? 1 : 2;   /* parity choice of the next digit */
        seq_t new_val = prev + d * pow5_val;  /* numerator before division */
        new_val = new_val >> 1;               /* divide by 2 */
        cache[m] = new_val;
        cache_len = m + 1;
        pow5_val *= 5;                       /* update 5^m for next step */
    }

    return cache[n-1];
}
