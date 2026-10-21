/*
 * A078479  a(1)=1, a(n+1)=a(lpf(n))+a(gpf(n)), where lpf(n) and gpf(n) are
 *          the least and greatest prime factors of n (A020639, A006530).
 * https://oeis.org/A078479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n+1) = a(lpf(n)) + a(gpf(n)) lpf(n) = least prime
 *            factor of n, gpf(n) = greatest prime factor of n (for n = 1,
 *            lpf(1) = gpf(1) = 1)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* static cache of computed values */
static seq_t *a_cache      = NULL;   /* array indexed [1..] */
static size_t a_cap       = 0;       /* allocated capacity */
static size_t a_len       = 0;       /* maximum index for which a_cache is filled */

/* static arrays for least and greatest prime factors */
static uint64_t *lpf   = NULL;       /* least prime factor, up to lpf_cap */
static uint64_t *gpf   = NULL;       /* greatest prime factor, up to gpf_cap */
static size_t      lpf_cap = 0;

/* forward declarations */
static void ensure_capacity_lpf(size_t n);
static void recompute_sieve(size_t n);
static void ensure_a_cache(size_t n);

/* Ensure lpf and gpf arrays accommodate indices up to n (inclusive) */
static void ensure_capacity_lpf(size_t n)
{
    if (lpf_cap >= n + 1) return;
    /* allocate new arrays and copy existing data */
    size_t new_cap = (n + 1) * 2;
    uint64_t *new_lpf = (uint64_t*)malloc(new_cap * sizeof(uint64_t));
    uint64_t *new_gpf = (uint64_t*)malloc(new_cap * sizeof(uint64_t));
    if (lpf) memcpy(new_lpf, lpf, lpf_cap * sizeof(uint64_t));
    if (gpf) memcpy(new_gpf, gpf, lpf_cap * sizeof(uint64_t));
    free(lpf); free(gpf);
    lpf = new_lpf;
    gpf = new_gpf;
    lpf_cap = new_cap;
}

/* Compute lpf and gpf sieve up to n */
static void recompute_sieve(size_t n)
{
    ensure_capacity_lpf(n);
    /* initialise */
    lpf[1] = gpf[1] = 1;
    for (size_t i = 2; i <= n; ++i) {
        lpf[i] = gpf[i] = i;
    }
    /* sieve */
    for (size_t i = 2; i <= n; ++i) {
        if (lpf[i] == i) {              /* i is prime */
            for (size_t j = i; j <= n; j += i) {
                if (lpf[j] > i) lpf[j] = i;
                gpf[j] = i;              /* largest prime divisor so far */
            }
        }
    }
}

/* Ensure a_cache is allocated & has values up to n
   assumes lpf & gpf are already computed up to n   */
static void ensure_a_cache(size_t n)
{
    if (a_len >= n) return;
    if (a_cap < n + 1) {
        /* allocate new cache */
        size_t new_cap = (n + 1) * 2;
        seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (a_cache) memcpy(new_arr, a_cache, a_cap * sizeof(seq_t));
        free(a_cache);
        a_cache = new_arr;
        a_cap = new_cap;
    }
    /* initialise first element if needed */
    if (a_len == 0) {
        a_cache[1] = (seq_t)1;
        a_len = 1;
    }
    /* compute missing values sequentially */
    for (size_t i = a_len + 1; i <= n; ++i) {
        size_t n_ = i - 1;                      /* recurrence uses a(n) for n = i-1 */
        size_t lp = lpf[n_];                    /* least prime factor */
        size_t gp = gpf[n_];                    /* greatest prime factor */
        a_cache[i] = a_cache[lp] + a_cache[gp]; /* add in seq_t type */
    }
    a_len = n;
}

seq_t a(idx_t n)
{
    /* redirect idx_t to size_t for array indexing (safe for our limits) */
    size_t idx = (size_t)n;
    if (idx == 0) return (seq_t)0; /* undefined but safe */

    /* ensure sieve and cache cover the requested index */
    recompute_sieve(idx);       /* lpf/gpf up to idx */
    ensure_a_cache(idx);        /* compute a[n] if necessary */
    return a_cache[idx];
}
