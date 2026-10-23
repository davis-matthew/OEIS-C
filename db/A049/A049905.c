/*
 * A049905  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 3, where m =
 *          2^(p+1) + 2 - n and p is the unique number such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = 1 and a(2) = 2.
 * https://oeis.org/A049905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2.
   For n ≥ 3, let p be the unique integer with 2^p < n-1 ≤ 2^(p+1).
   Then m = 2^(p+1) + 2 - n and
   a(n) = (a(1)+...+a(n-1)) - a(m). */
/* The implementation uses memoised prefix sums. */
#include <stdlib.h>

static inline unsigned floor_log2_u64(idx_t v)
{
    unsigned r = 0;
    while (v >>= 1) ++r;
    return r;
}

static seq_t *a_cache = NULL;          /* a[0..] */
static seq_t *pref_cache = NULL;       /* prefix sums pref[0..] */
static size_t cache_size = 0;          /* allocated size */
static size_t last_computed = 0;       /* highest index computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* n<1 is not used */

    /* Allocate and initialise the arrays on the first call. */
    if (a_cache == NULL) {
        cache_size = 4;
        a_cache = (seq_t*)calloc(cache_size, sizeof(seq_t));
        pref_cache = (seq_t*)calloc(cache_size, sizeof(seq_t));
        if (a_cache == NULL || pref_cache == NULL) return 0; /* allocation failed */
        a_cache[0] = (seq_t)0;
        pref_cache[0] = (seq_t)0;
    }

    /* Grow the arrays if needed. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_cap = cache_size;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;
        a_cache = tmp;
        tmp = (seq_t*)realloc(pref_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;
        pref_cache = tmp;
        cache_size = new_cap;
    }

    /* Compute terms up to n. */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        idx_t idx = (idx_t)i;
        if (idx == 1) {
            a_cache[i] = (seq_t)1;
            pref_cache[i] = (seq_t)1;
        } else if (idx == 2) {
            a_cache[i] = (seq_t)2;
            pref_cache[i] = pref_cache[i-1] + (seq_t)2;
        } else {
            unsigned p = floor_log2_u64(idx - 2);
            seq_t pow2 = ((seq_t)1) << (p + 1);      /* 2^(p+1) */
            idx_t m = (idx_t)(pow2 + (seq_t)2 - idx);
            /* m is always ≥1 for n≥3 */
            seq_t val = pref_cache[i-1] - a_cache[(size_t)m];
            a_cache[i] = val;
            pref_cache[i] = pref_cache[i-1] + val;
        }
    }
    last_computed = (size_t)n;

    return a_cache[n];
}
