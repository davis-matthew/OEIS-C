/*
 * A020928  Expansion of 1/(1-4*x)^(17/2).
 * https://oeis.org/A020928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = a(n-1) * (2*n+16)*(2*n+15) / (n*(n+8)) for n ≥
 *            1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 21, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --- local utilities ---------------------------------------------------- */
static seq_t seq_gcd(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --- memoisation table ------------------------------------------------- */
static seq_t *cache = NULL;        /* array of a(k) */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t  cached_n  = 0;       /* largest n that has been computed */

/* ensure that cache[n] exists (allocates or grows the table) */
static void ensure_cache_capacity(idx_t n) {
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (n + 1) * 2;
        if (new_cap == 0) new_cap = 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();          /* allocation failed */
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* compute values for all indices > cached_n up to n (inclusive) */
static void compute_up_to(idx_t n) {
    for (idx_t k = cached_n + 1; k <= n; ++k) {
        seq_t prev  = cache[(size_t)k - 1];
        seq_t num1  = (seq_t)(2 * k + 16);   /* 2n+16 */
        seq_t num2  = (seq_t)(2 * k + 15);   /* 2n+15 */
        seq_t den1  = (seq_t)k;              /* n     */
        seq_t den2  = (seq_t)(k + 8);        /* n+8   */

        /* step 1: cancel common factors in the denominator with the
           previous value (a_k-1) */
        seq_t g = seq_gcd(prev, den1);
        prev /= g;
        den1  /= g;

        g = seq_gcd(prev, den2);
        prev /= g;
        den2  /= g;

        /* step 2: cancel remaining denominator factors with numerators */
        g = seq_gcd(num1, den1);
        num1 /= g;
        den1  /= g;

        g = seq_gcd(num2, den2);
        num2 /= g;
        den2  /= g;

        g = seq_gcd(num1, den2);
        num1 /= g;
        den2  /= g;

        g = seq_gcd(num2, den1);
        num2 /= g;
        den1  /= g;

        /* at this point den1 and den2 should both be 1 */
        /* multiply the remaining factors in a safe order */
        seq_t prod = prev * num1;   /* the product fits in seq_t for
                                      all n for which a(n) fits in seq_t   */
        prod = prod * num2;

        /* Since the division was exact, any remaining division must
           be trivial (den1 and den2 are 1). */
        cache[(size_t)k] = prod;
    }
    cached_n = n;
}

/* public function -------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == (idx_t)0) {
        if (cache_cap == 0) {
            ensure_cache_capacity(0);
            cache[0] = (seq_t)1;
            cached_n = 0;
        }
        return cache[0];
    }

    ensure_cache_capacity(n);
    if (n > cached_n) compute_up_to(n);
    return cache[(size_t)n];
}
