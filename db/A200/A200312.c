/*
 * A200312  a(n) = A000108(n)*A006130(n), where A000108 is the Catalan numbers
 *          and A006130(n) = A006130(n-1) + 3*A006130(n-2).
 * https://oeis.org/A200312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000108(n) * A006130(n) where A006130 satisfies S(0)=1,
 *            S(1)=1, S(n)=S(n-1)+3*S(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   catalan, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the provided catalan(idx_t) and ensure_cache(idx_t, seq_t **, size_t *) helpers. */

#include <stddef.h>
#include "../utils/catalan.h"
#include "../utils/ensure_cache.h"

static seq_t *s_cache = NULL;      /* cache for A006130 values */
static size_t s_cap = 0;           /* current allocated size of s_cache */
static idx_t  s_max = (idx_t)-1;   /* highest index computed in s_cache */

/* back‑end helper to grow s_cache if needed */
static void grow_s_cache(idx_t n) {
    /* ensure_cache expands the array to at least n+1 elements */
    ensure_cache(n, &s_cache, &s_cap);
    /* initialise the base values if they have not been set */
    if (s_cache[0] == 0) s_cache[0] = 1;
    if (n >= 1 && s_cache[1] == 0) s_cache[1] = 1;
    /* compute missing entries using the recurrence */
    if (s_max == (idx_t)-1) {
        /* no elements computed yet */
        s_max = 1;
    }
    if ((idx_t)s_max < n) {
        for (idx_t i = s_max + 1; i <= n; ++i) {
            seq_t a = s_cache[i - 1];
            seq_t b = s_cache[i - 2];
            s_cache[i] = a + 3 * b;
        }
        s_max = n;
    }
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n) {
    if (n == 0 && s_max < 0) {            /* first call, initialise cache */
        grow_s_cache(0);
    } else {
        grow_s_cache(n);
    }
    seq_t s = s_cache[n];
    seq_t c = catalan(n);
    return c * s;
}
