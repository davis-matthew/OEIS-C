/*
 * A048715  Binary expansion matches (100(0)*)*(0|1|10)?; or, Zeckendorf-like
 *          expansion of n using recurrence f(n) = f(n-1) + f(n-3).
 * https://oeis.org/A048715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^(invfoo(n)-4) + a(n - foo(invfoo(n))) where foo(0)=0,
 *            foo(1)=0, foo(2)=1 and foo(k)=foo(k-1)+foo(k-3) for k>=3. a(0)=0
 * Range:     u8 n <= 27, u16 n <= 594, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t *foo_cache     = NULL;   /* cached foo sequence values   */
static size_t foo_cache_size = 0;     /* number of elements stored   */
static size_t foo_cache_cap  = 0;     /* allocated capacity          */

/* ensure the cache is initialized with the first three values */
static void ensure_foo_initialized(void)
{
    if (!foo_cache) {
        foo_cache_cap = 32;
        foo_cache = (idx_t *)malloc(foo_cache_cap * sizeof(idx_t));
        if (!foo_cache) exit(EXIT_FAILURE);
        foo_cache[0] = 0;
        foo_cache[1] = 0;
        foo_cache[2] = 1;
        foo_cache_size = 3;
    }
}

/* enlarge the cache if necessary */
static void ensure_foo_capacity(size_t new_cap)
{
    if (foo_cache_cap < new_cap) {
        size_t cap = foo_cache_cap ? foo_cache_cap * 2 : 4;
        while (cap < new_cap) cap *= 2;
        idx_t *tmp = (idx_t *)realloc(foo_cache, cap * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);
        foo_cache = tmp;
        foo_cache_cap = cap;
    }
}

/* grow the cache until it contains at least one term > n */
static void ensure_foo_up_to(idx_t n)
{
    ensure_foo_initialized();
    while (foo_cache_size > 0 && foo_cache[foo_cache_size-1] <= n) {
        idx_t next;
        if (foo_cache_size >= 3)
            next = foo_cache[foo_cache_size-1] + foo_cache[foo_cache_size-3];
        else
            next = 0; /* not used for indices < 3          */
        ensure_foo_capacity(foo_cache_size + 1);
        foo_cache[foo_cache_size] = next;
        foo_cache_size++;
    }
}

/* inverse of foo: largest i with foo(i) <= n */
static inline idx_t invfoo(idx_t n)
{
    /* foo_cache[foo_cache_size-1] > n                           */
    idx_t lo = 0;
    idx_t hi = foo_cache_size - 1;
    idx_t best = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (foo_cache[mid] <= n) {
            best = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return best;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    ensure_foo_up_to(n);

    seq_t result = 0;

    while (n > 0) {
        idx_t i = invfoo(n);               /* i >= 4 for n > 0 */
        idx_t exp = i - 4;

        /* compute 2^exp as seq_t */
        seq_t term = (seq_t)1;
        for (idx_t j = 0; j < exp; ++j) term <<= 1;

        result += term;
        n -= foo_cache[i];
    }

    return result;
}
