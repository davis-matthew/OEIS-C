/*
 * A223267  Rolling cube footprints: number of nX6 0..5 arrays starting with 0
 *          where 0..5 label faces of a cube and every array movement to a
 *          horizontal, diagonal or antidiagonal neighbor moves across a
 *          corresponding cube edge.
 * https://oeis.org/A223267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1152*a(n-1) -229376*a(n-2) -21233664*a(n-3)
 *            +4697620480*a(n-4) +12884901888*a(n-5) -17798344474624*a(n-6)
 *            +343047627866112*a(n-7) +6755399441055744*a(n-8)
 *            -90071992547409920*a(n-9) Initial values: a(1) … a(9) are as
 *            listed in the OEIS entry
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   seq_from_str, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Utility headers for arbitrary‑precision support (if needed) */
#include "../utils/seq_from_str.h"   /* seq_from_str(const char *s) -> seq_t */
#include "../utils/seq_mul.h"        /* seq_mul(seq_t, seq_t) -> seq_t */

/* Global state for memoisation */
static seq_t *cache = NULL;        /* 1‑based indexing; cache[0] unused */
static size_t cache_cap = 0;       /* number of allocated elements (cache_cap-1 is max index) */
static idx_t  max_computed = 0;    /* highest n that has been computed */

/* Recurrence coefficients (k=0 corresponds to a(n-1) etc.) */
static const long long coeff[9] = {
    1152LL,           /* +1152       * a(n-1) */
   -229376LL,           /* -229376     * a(n-2) */
   -21233664LL,          /* -21233664   * a(n-3) */
    4697620480LL,        /* +4697620480 * a(n-4) */
    12884901888LL,       /* +12884901888*a(n-5) */
   -17798344474624LL,    /* -17798344474624*a(n-6) */
    343047627866112LL,   /* +343047627866112*a(n-7) */
    6755399441055744LL,  /* +6755399441055744*a(n-8) */
   -90071992547409920LL  /* -90071992547409920*a(n-9) */
};

/* String representations of the first nine terms */
static const char *init_str[9] = {
    "1024",
    "737280",
    "611319808",
    "522106961920",
    "450204914417664",
    "389343801904201728",
    "337035427916688654336",
    "291846966499723716853760",
    "252743669925904582659014656"
};

/* Ensure the cache has room for at least indices up to n
   and compute any missing values up to n. */
static void ensure_cache(idx_t n) {
    /* Allocate or enlarge the cache */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1 > 10 ? n + 1 : 10);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* initialise element 0 to zero (unused) */
        cache[0] = (seq_t)0;
        /* copy the known first nine terms */
        for (size_t i = 0; i < 9; ++i)
            cache[i + 1] = seq_from_str(init_str[i]);
        max_computed = 9;
    } else if ((size_t)(n + 1) > cache_cap) {
        /* grow to at least n+1 elements */
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing values using the recurrence (only for n ≥ 10) */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* For i ≤ 9 the value is already stored as an initial value */
        if (i <= 9) continue;
        seq_t sum = (seq_t)0;
        for (int k = 0; k < 9; ++k) {
            seq_t term = cache[i - k - 1];
            seq_t prod = seq_mul(term, (seq_t)llabs(coeff[k]));
            if (coeff[k] >= 0)
                sum += prod;
            else
                sum -= prod;
        }
        cache[i] = sum;
    }
    if (max_computed < n) max_computed = n;
}

/* Public function: returns a(n) for n ≥ 1 (undefined for n < 1, returns 0). */
seq_t a(idx_t n) {
    if (n < (idx_t)1) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
