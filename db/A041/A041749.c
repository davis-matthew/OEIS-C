/*
 * A041749  Denominators of continued fraction convergents to sqrt(394).
 * https://oeis.org/A041749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 790046070*a(n-21) + a(n-42) (for n>41)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 47, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial terms are computed from the continued fraction of sqrt(394). */

#include <stdlib.h>
#include <stdint.h>

/* Global cache for the sequence */
static seq_t *cache = NULL;          /* pointer to array of terms */
static size_t cache_cap = 0;         /* current allocated capacity */
static idx_t cache_upto = 0;         /* highest index that has been computed */

/* Ensure the cache has space for at least indices 0 … needed */
static void ensure_cache(idx_t needed)
{
    if (needed < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* memory exhaustion */

    cache     = tmp;
    cache_cap = new_cap;
}

/* Initialize the first 42 terms (indices 0 … 41) from the continued fraction
   of sqrt(394).  The sequence of convergent denominators satisfies the
   recurrence and the values here are the seed for the long linear recurrence. */
static void init_cache(void)
{
    /* 394 has integer part 19 */
    const uint64_t D   = 394ULL;
    const uint64_t a0  = 19ULL;      /* floor(sqrt(394)) */

    uint64_t m     = 0;
    uint64_t d     = 1;
    uint64_t a     = a0;

    /* q_{-1} = 0, q_0 = 1  */
    seq_t q_prev = 0;   /* q_{-1} */
    seq_t q_curr = 1;   /* q_0   */

    cache[0] = q_curr;           /* a(0) = 1 */

    for (idx_t k = 1; k <= 41; ++k) {
        /* Compute next partial quotient a_{k} */
        uint64_t m_next = d * a - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;

        /* Denominator recurrence: q_{k} = a_{k} * q_{k-1} + q_{k-2} */
        seq_t a_seq = (seq_t)a;
        seq_t q_next = a_seq * q_curr + q_prev;

        cache[k] = q_next;

        q_prev = q_curr;
        q_curr = q_next;
    }

    cache_upto = 41;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (!cache) {
        ensure_cache(42);          /* at least space for indices 0 … 41 */
        init_cache();
    }

    /* If the requested index has already been computed, return it. */
    if (n <= cache_upto)
        return cache[n];

    /* Otherwise extend the cache up to n using the long recurrence. */
    ensure_cache(n);

    const seq_t C = (seq_t)790046070;   /* multiplier in the recurrence */

    for (idx_t k = cache_upto + 1; k <= n; ++k) {
        /* a(k) = C * a(k-21) + a(k-42)   (k > 41) */
        seq_t term = C * cache[k - 21] + cache[k - 42];
        cache[k] = term;
    }

    cache_upto = n;
    return cache[n];
}
