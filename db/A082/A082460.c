/*
 * A082460  a(n) = pi(n) - a(n - 1) where pi(n) = A000720(n).
 * https://oeis.org/A082460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n) - a(n-1) with a(1) = 0 ; for n ≥ 2, a(n) = pi(n) -
 *            a(n-1) (pi(n) is the prime counting function A000720)
 * Range:     u8 n <= 1620, u16 n <= 9830, u32 n <= 9830, u64 n <= 9830,
 *            u128 n <= 9830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Local helper: compute the prime counting function pi(n). */
/* ------------------------------------------------------------------ */
static seq_t _pi_of(idx_t n)
{
    /* Static sieve state. */
    static idx_t  sieve_limit = 0;                 /* largest index sieved so far */
    static bool  *is_comp    = NULL;                /* composite flag array */
    static size_t *pi_cache  = NULL;                /* pi values from 0..sieve_limit */

    /* Initialise for the first call.  pi(0)=pi(1)=0. */
    if (sieve_limit == 0) {
        is_comp  = (bool *)calloc(2, sizeof(bool));   /* indices 0,1 */
        pi_cache = (size_t *)calloc(2, sizeof(size_t));
        if (!is_comp || !pi_cache) exit(1);
        pi_cache[0] = 0;
        pi_cache[1] = 0;
        sieve_limit = 1;
    }

    if (n <= sieve_limit)
        return (seq_t)pi_cache[n];

    idx_t old_limit = sieve_limit;
    idx_t new_limit = n;

    /* Extend arrays to new_limit. */
    bool *new_is_comp = (bool *)realloc(is_comp, (new_limit + 1) * sizeof(bool));
    size_t *new_pi_cache = (size_t *)realloc(pi_cache, (new_limit + 1) * sizeof(size_t));
    if (!new_is_comp || !new_pi_cache) exit(1);
    is_comp = new_is_comp;
    pi_cache = new_pi_cache;

    /* Initialize the newly allocated part of is_comp to false (prime) */
    if (new_limit > old_limit) {
        /* New elements start at index old_limit+1 up to new_limit inclusive. */
        size_t bytes_to_clear = (new_limit - old_limit) * sizeof(bool);
        memset(is_comp + old_limit + 1, 0, bytes_to_clear);
    }

    /* Sieve the new range.  We must examine all i up to new_limit,
       because some of the new numbers may be primes that were
       unmarked previously. */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (!is_comp[i]) {                      /* i is prime */
            idx_t start = i * i;
            if (start < i)                      /* overflow guard, should not happen here */
                start = new_limit + 1;
            for (idx_t j = start; j <= new_limit; j += i)
                is_comp[j] = true;
        }
    }

    /* Compute pi values for the new indices. */
    for (idx_t i = old_limit + 1; i <= new_limit; ++i)
        pi_cache[i] = pi_cache[i - 1] + ((!is_comp[i] && i >= 2) ? 1 : 0);

    sieve_limit = new_limit;
    return (seq_t)pi_cache[n];
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Undefined behaviour for n < 1; return 0 safely. */
    if (n < 1) return (seq_t)0;

    /* Static cache for a(1) … a(n).  cache[0] holds a(1). */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;   /* number of elements currently stored */

    /* Initialise cache if it is empty. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(1 * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)0;        /* a(1) = 0 */
        cache_len = 1;
    }

    /* Grow cache if necessary. */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;

        for (idx_t i = cache_len; i < n; ++i) {   /* compute a(i+1) */
            /* i indexes from cache_len to n-1, corresponding to n_i = i+1 */
            seq_t pi_val = _pi_of(i + 1);          /* pi(i+1) */
            cache[i] = pi_val - cache[i - 1];
        }
        cache_len = n;
    }

    return cache[n - 1];
}
