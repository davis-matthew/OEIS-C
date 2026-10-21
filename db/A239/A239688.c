/*
 * A239688  Number of paths joining opposite corners of an n X 2 grid with
 *          every vertex appearing at most twice in the path.
 * https://oeis.org/A239688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) for n≥13 satisfies the recurrence a(n) = 8*a(n-1) +
 *            26*a(n-2) – 80*a(n-3) – 31*a(n-4) + 252*a(n-5) – 129*a(n-6) –
 *            268*a(n-7) + 197*a(n-8) + 116*a(n-9) – 71*a(n-10) – 20*a(n-11) +
 *            4*a(n-12) with initial values a(1)=2, a(2)=22, a(3)=218,
 *            a(4)=2100, a(5)=20701, a(6)=202664, a(7)=1990006, a(8)=1...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* A cache of previously computed terms.
       The cache grows only, never shrinks. */
    static seq_t *cache      = NULL;   /* array of seq_t, 1‑based */
    static size_t cache_cap  = 0;      /* number of allocated slots (≥1) */
    static idx_t  last_computed = 0;   /* largest n for which cache[n] is set */

    /* Recurrence coefficients for a(n)=∑_{k=1}^{12} c_k * a(n-k)  (k=1→12) */
    static const int coeffs[13] = {
        0,          /* unused for index 0 */
        8,  26, -80, -31, 252, -129, -268,  197,  116,  -71,  -20,   4
    };

    /* Initial terms a(1) … a(12) */
    static const seq_t init[13] = {
        (seq_t)0ULL,             /* placeholder for a(0) */
        (seq_t)2ULL,             (seq_t)22ULL,            (seq_t)218ULL,
        (seq_t)2100ULL,          (seq_t)20701ULL,        (seq_t)202664ULL,
        (seq_t)1990006ULL,       (seq_t)19519656ULL,     (seq_t)191538146ULL,
        (seq_t)1879225062ULL,    (seq_t)18438413081ULL,  (seq_t)180909222618ULL
    };

    /* Ensure the cache can hold at least the first 12 terms (index up to 12). */
    if (cache_cap < 13) {
        size_t new_cap = 13;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
        /* Initialise only once */
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = init[i];
        last_computed = 12;
    }

    /* If larger n than cached, grow cache. */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If request is for a term already computed, return it. */
    if (n <= last_computed)
        return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (int k = 1; k <= 12; ++k)
            sum += (seq_t)coeffs[k] * cache[m - k];
        cache[m] = sum;
    }
    last_computed = n;
    return cache[n];
}
