/*
 * A053856  Number of n X n matrices over GF(4) of order dividing 2 (i.e.,
 *          number of solutions of X^2=I in GL(n,4)).
 * https://oeis.org/A053856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{r=0..⌊n/2⌋} GaussianBinomial(n,r,4) *
 *            GaussianBinomial(n−r,r,4) * |GL(r,4)|
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses only C++-compatible features (malloc, realloc) and
 * works for both C and C++ builds.  When seq_t is a class type,
 * pointers to allocated memory are cast from void* to seq_t*.
 * All arithmetic is performed using seq_t's operator overloads or
 * built‑in arithmetic for the usual integer types. */

#include <stdlib.h>   /* malloc, realloc, exit */

/* Dynamic array of powers of 4: pow4_arr[k] = 4^k. */
static seq_t *pow4_arr = NULL;  /* pointer to array of seq_t */
static size_t pow4_cap = 0;
static size_t pow4_len = 0;

/* Ensure that pow4_arr contains 4^k for 0 ≤ k ≤ n. */
static void ensure_pow4_up_to(idx_t n)
{
    /* Initialise the array on first call. */
    if (pow4_arr == NULL) {
        pow4_arr = (seq_t *)malloc(sizeof(seq_t));
        if (!pow4_arr) exit(1);
        pow4_cap = 1;
        pow4_len = 1;
        pow4_arr[0] = (seq_t)1;                /* 4^0 */
    }

    size_t need = (size_t)n + 1;
    if (pow4_len < need) {
        size_t new_cap = pow4_cap ? pow4_cap : 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(pow4_arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        pow4_arr = tmp;
        pow4_cap = new_cap;
        for (idx_t i = pow4_len; i < need; ++i)
            pow4_arr[i] = pow4_arr[i - 1] * (seq_t)4;
        pow4_len = need;
    }
}

/* Gaussian binomial coefficient  C_q(n,k) with q=4. */
static seq_t gaussian_binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numer = pow4_arr[n - k + i] - (seq_t)1;
        seq_t denom = pow4_arr[i] - (seq_t)1;
        result = result * numer / denom;
    }
    return result;
}

/* |GL(r,4)| = ∏_{i=0}^{r-1} (4^r − 4^i). */
static seq_t GL_of_r(idx_t r)
{
    if (r == 0) return (seq_t)1;
    seq_t pow4r = pow4_arr[r];
    seq_t res = (seq_t)1;
    seq_t pow4i = (seq_t)1;      /* 4^0 */
    for (idx_t i = 0; i < r; ++i) {
        res = res * (pow4r - pow4i);
        pow4i = pow4i * (seq_t)4;
    }
    return res;
}

/* Array of previously computed a(n) values.  Index 0 is unused. */
static seq_t *cache_arr = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;   /* number of valid entries (>=1) */

/* Initialise the cache on first call. */
static void init_cache(void)
{
    if (cache_arr != NULL) return;              /* already initialised */
    cache_arr = (seq_t *)malloc(sizeof(seq_t));
    if (!cache_arr) exit(1);
    cache_cap = 1;
    cache_len = 1;                              /* cache_arr[0] is dummy */
    cache_arr[0] = (seq_t)0;
}

/* ------------------------------------------------------------ */
/* Main entry function for the sequence.                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_cache();
    if (n < cache_len) return cache_arr[n];

    /* Make sure 4^k up to n is available for all needed calculations. */
    ensure_pow4_up_to(n);

    /* Grow cache array if necessary. */
    if (cache_len <= n) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache_arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache_arr = tmp;
        cache_cap = new_cap;
    }

    /* Compute values for all missing indices. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t max_r = i >> 1;            /* floor(i/2) */
        for (idx_t r = 0; r <= max_r; ++r) {
            seq_t g1 = gaussian_binom(i, r);
            seq_t g2 = gaussian_binom(i - r, r);
            seq_t gl = GL_of_r(r);
            sum += g1 * g2 * gl;
        }
        cache_arr[i] = sum;
    }
    cache_len = n + 1;
    return cache_arr[n];
}
