/*
 * A231946  Partial sums of the third power of the arithmetic derivative
 *          function A003415.
 * https://oeis.org/A231946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=1..n} (j')^3 where j' is the arithmetic derivative
 *            of j
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 287, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of types supplied by the build */
typedef uint64_t idx_t;

extern seq_t (*seq_t_placeholder)(void); /* dummy to avoid unused type warnings */

/* Internal state for memoisation */
static seq_t *cache     = NULL;    /* Cache of prefix sums a(k) for 0 ≤ k ≤ max_cached */
static size_t cache_cap = 0;      /* Allocated size of cache (elements) */
static idx_t  max_cached = 0;     /* Highest n that has been computed */

/* Smallest prime factor table */
static uint64_t *spf      = NULL; /* spf[i] = smallest prime factor of i, for 0 ≤ i ≤ spf_cap-1 */
static size_t   spf_cap   = 0;    /* Allocated size of spf (elements) */

/* ------------------------------------------------------------
 * Recompute or extend the smallest prime factor table up to `limit`.
 * ------------------------------------------------------------ */
static void sieve_spf(idx_t limit)
{
    if (limit + 1 <= spf_cap)
        return;

    /* Allocate bigger table */
    uint64_t *new_spf = (uint64_t *)realloc(spf, (size_t)(limit + 1) * sizeof(uint64_t));
    if (!new_spf) /* out of memory */
        return;
    spf       = new_spf;
    spf_cap   = (size_t)(limit + 1);

    /* Initialise */
    for (size_t i = 0; i <= limit; ++i)
        spf[i] = 0;
    if (limit >= 1)
        spf[1] = 1;   /* 1 has no prime factor */

    /* Sieve: fill smallest prime factor for each integer */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            if (i > limit / 2)        /* avoid overflow in j += i loop */
                continue;
            for (idx_t j = i + i; j <= limit; j += i) {
                if (spf[j] == 0)
                    spf[j] = (uint64_t)i;
            }
        }
    }
}

/* ------------------------------------------------------------
 * Compute the arithmetic derivative d for integer n using the
 * precomputed spf table.  The result is returned as seq_t.
 * ------------------------------------------------------------ */
static seq_t arithmetic_derivative(idx_t n)
{
    seq_t der = 0;
    idx_t temp = n;

    while (temp > 1) {
        idx_t p = (idx_t)spf[temp];
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        /* der += (n / p) * e */
        der += (seq_t)(n / p) * e;
    }
    return der;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n=0, return 0 */
        return 0;

    /* Ensure we have a spare cache entry for index 0 */
    if (cache_cap == 0) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;      /* out of memory fallback */
        cache[0] = 0;              /* a(0) = 0 for convenience */
        max_cached = 0;
    }

    /* Expand cache if needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* out of memory fallback */
        cache     = tmp;
        cache_cap = new_cap;
        /* new entries will be filled in the loop below */
    }

    /* Compute values that are not yet cached */
    sieve_spf(n);                 /* make sure we have spf up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t der   = arithmetic_derivative(i);
        seq_t der3  = der * der * der;           /* (i')^3 */
        cache[i] = cache[i - 1] + der3;           /* partial sum */
    }
    max_cached = n;

    return cache[n];
}
