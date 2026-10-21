/*
 * A057827  a(0) = 1; a(n) = LCM(n, sum{k=0 to n-1}[a(k)]).
 * https://oeis.org/A057827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM(n, ∑_{k=0}^{n-1} a(k)) Initial value: a(0) = 1 The
 *            implementation uses a simple Euclidean algorithm for GCD and
 *            caches previously computed terms and the running sum. All
 *            arithmetic is performed in the native seq_t type, which may be
 *            an 8‑, 16‑, 32‑, 64‑bit unsigned integer or an arbitr...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute gcd of two values of type seq_t.  The loop terminates
   when b becomes zero; for unsigned types the division is exact. */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Cache of computed terms.  cache_len stores the number of
   terms currently computed and cache_cap the allocated capacity. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Running sum of a(0) + … + a(n-1).  It is updated when a new term
   is computed.  The type seq_t is used so that the sum is exact
   for arbitrary‑precision builds. */
static seq_t sum = (seq_t)0;

/* Ensure that cache[0..n] exist; grow the cache as needed. */
static void ensure_terms(idx_t n)
{
    /* Initialise on first call.  a(0) = 1. */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        sum      = (seq_t)1;
        cache_len = 1;
    }

    /* Compute missing values up to index n. */
    while (cache_len <= (size_t)n)
    {
        /* Grow cache if necessary. */
        if (cache_len >= cache_cap)
        {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        idx_t   i      = (idx_t)cache_len;   /* index of new term */
        seq_t  n_seq   = (seq_t)i;           /* n cast to seq_t   */
        seq_t  g       = gcd_seq_t(n_seq, sum);
        seq_t  mult    = n_seq / g;          /* n / gcd(n,sum)    */
        seq_t  ai      = mult * sum;         /* lcm(n, sum)       */

        cache[cache_len] = ai;
        sum += ai;                           /* update running sum */
        ++cache_len;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[n];
}
