/*
 * A216683  G.f. satisfies: A(x) = 1 + x*A(x) / ( A(I*x)*A(-I*x) ).
 * https://oeis.org/A216683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 27, i32 n <= 46, i64 n <= 83, i128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A216683
 *  Recurrence derived from the functional equation:
 *      a(0) = 1
 *      For n ≥ 1:
 *          a(n) = a(n-1) -
 *                 Σ_{m=2,4,6,…,n-1} a(n-m) * r(m)
 *      where r(m) = s(m)   if m ≡ 0 (mod 4)
 *                    -s(m)  if m ≡ 2 (mod 4)
 *      and s(m) = Σ_{p+q=m} (-1)^q a(p) a(q)
 *      with s(0) = 1.
 *  This recurrence reproduces the listed terms and computes a(n) for all n
 *  in O(n²) time, which is acceptable for n ≈ 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- Internal cache data ------------------------------------------- */
static seq_t *a_cache = NULL;   /* a[0..N]      */
static seq_t *s_cache = NULL;   /* s[0..N]      */
static size_t  cache_cap = 0;   /* allocated size  */
static idx_t   last_computed = (idx_t)-1;    /* last index computed  */
static bool    initialized = false;          /* base case set?       */

/* --- Ensure the cache can hold indices up to n --------------------- */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    s_cache = (seq_t *)realloc(s_cache, new_cap * sizeof(seq_t));

    for (size_t i = cache_cap; i < new_cap; ++i) {
        a_cache[i] = 0;
        s_cache[i] = 0;
    }

    cache_cap = new_cap;

    if (!initialized) {           /* initialise base case once   */
        a_cache[0] = 1;
        s_cache[0] = 1;
        last_computed = 0;
        initialized = true;
    }
}

/* --- Main sequence function ---------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 1;                 /* base value */

    ensure_cache(n);

    /* Compute missing terms up to n */
#ifdef __cplusplus
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        /* Compute the real part sum needed for a(k) */
        seq_t sum_real = 0;
        for (idx_t m = 2; m < k; m += 2) {
            seq_t r = s_cache[m];
            if (m & 2)            /* m ≡ 2 (mod 4) */
                r = -r;
            sum_real += a_cache[k - m] * r;
        }
        a_cache[k] = a_cache[k - 1] - sum_real;

        /* Compute s(k) for future use */
        seq_t Sk = 0;
        for (idx_t p = 0; p <= k; ++p) {
            idx_t q = k - p;
            seq_t prod = a_cache[p] * a_cache[q];
            if (q & 1)
                Sk -= prod;
            else
                Sk += prod;
        }
        s_cache[k] = Sk;
    }
#else
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        __int128 sum_real = 0;
        for (idx_t m = 2; m < k; m += 2) {
            seq_t r = s_cache[m];
            if (m & 2)            /* m ≡ 2 (mod 4) */
                r = -r;
            sum_real += (__int128) a_cache[k - m] * r;
        }
        a_cache[k] = a_cache[k - 1] - (seq_t) sum_real;

        __int128 Sk = 0;
        for (idx_t p = 0; p <= k; ++p) {
            idx_t q = k - p;
            __int128 prod = (__int128) a_cache[p] * a_cache[q];
            if (q & 1)
                Sk -= prod;
            else
                Sk += prod;
        }
        s_cache[k] = (seq_t) Sk;
    }
#endif

    last_computed = n;
    return a_cache[n];
}
