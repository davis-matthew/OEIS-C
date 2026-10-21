/*
 * A204650  Number of (n+1) X 8 0..1 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A204650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 27*a(n-2) + 48*a(n-3) - 42*a(n-4) + 42*a(n-6)
 *            - 48*a(n-7) + 27*a(n-8) - 8*a(n-9) + a(n-10) for n > 15, with
 *            initial values a(1)–a(15) given in the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 54, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper for allocating an array of seq_t (works in C and C++). */
static seq_t *alloc_seq_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* Helper for freeing the array of seq_t. */
static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

static seq_t *cache = NULL;
static size_t cap = 0;      /* allocated capacity */
static size_t len = 0;      /* highest index computed */

/* Ensure cache has capacity for indices up to need (1‑based). */
static void ensure_capacity(size_t need)
{
    if (cap >= need) return;

    size_t new_cap = (cap > 0) ? cap : 16;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_cache = alloc_seq_array(new_cap + 1); /* +1 to use 1‑based index */
    if (!new_cache) return; /* allocation failed, rely on old cache */

    /* Copy existing values */
    for (size_t i = 0; i <= len; ++i) new_cache[i] = cache[i];

    /* Zero the rest (for safety, though not strictly needed) */
    for (size_t i = len + 1; i <= new_cap; ++i) new_cache[i] = 0;

    /* Free old cache */
    if (cache) free_seq_array(cache);

    cache = new_cache;
    cap   = new_cap;
}

/* Initialise cache with the first 15 terms if it hasn't been done yet. */
static void init_cache(void)
{
    if (cache) return; /* already initialised */

    /* Initial capacity large enough for first 15 terms */
    cap = 16;
    cache = alloc_seq_array(cap + 1); /* 1‑based indexing */
    if (!cache) return; /* allocation failed */

    for (size_t i = 0; i <= cap; ++i) cache[i] = 0; /* safety */

    /* First 15 terms (index 1..15) */
    cache[1] = 216;
    cache[2] = 283;
    cache[3] = 637;
    cache[4] = 1478;
    cache[5] = 3261;
    cache[6] = 6780;
    cache[7] = 13314;
    cache[8] = 24862;
    cache[9] = 44426;
    cache[10] = 76378;
    cache[11] = 126906;
    cache[12] = 204583;
    cache[13] = 321038;
    cache[14] = 491781;
    cache[15] = 737163;

    len = 15;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1 */

    init_cache(); /* ensures cache exists */

    /* If requested value already computed, return it. */
    if (n <= len) return cache[n];

    /* Ensure cache capacity for new indices up to n. */
    ensure_capacity((size_t)n);

    /* Coefficients for the recurrence (1‑based index k). */
    static const int  coeff[11] = {0,  8, -27,  48, -42,   0,
                                   42, -48,  27,  -8,   1};
    static const int  abs_coeff[11] = {0, 8, 27, 48, 42, 0,
                                       42, 48, 27, 8, 1};

    /* Compute terms from len+1 up to n. */
    for (idx_t i = len + 1; i <= n; ++i)
    {
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 10; ++k)
        {
            idx_t idx = i - k;
            if (idx < 1) continue; /* terms before 1 are zero */

            seq_t val = cache[idx];
            seq_t prod = val * ((seq_t)abs_coeff[k]);

            if (coeff[k] > 0) pos += prod;
            else             neg += prod;  /* coeff[k] < 0 */
        }
        /* pos >= neg for all valid n, so subtraction is safe. */
        cache[i] = pos - neg;
    }

    len = (size_t)n;
    return cache[n];
}
