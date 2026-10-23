/*
 * A164274  (n-th isolated prime) plus (n-th isolated composite).
 * https://oeis.org/A164274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = A007510(n) + A014574(n) (sum of the n-th isolated prime
 *            and the n-th isolated composite)
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Dynamic sieve helpers                                               */
/* --------------------------------------------------------------------- */

static idx_t   _limit          = 0;     /* current sieve limit           */
static bool   * _is_prime      = NULL;  /* sieve table, true if prime    */
static idx_t  * _primes        = NULL;  /* list of primes in ascending   */
static size_t   _prime_count   = 0;     /* number of primes stored       */

/* Rebuilds the sieve up to LIMIT (inclusive).  Caller ensures that the
   new LIMIT is larger than the current _limit.  All previous data are freed. */
static void _ensure_limit(idx_t limit)
{
    if (limit <= _limit)
        return;

    /* Allocate the new sieve array (explicit cast for C++ compatibility) */
    bool *new_is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (new_is_prime == NULL)
        abort();

    for (idx_t i = 0; i <= limit; ++i)
        new_is_prime[i] = true;
    new_is_prime[0] = new_is_prime[1] = false;

    /* Classic sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (new_is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_is_prime[j] = false;
        }
    }

    /* Build the prime list */
    size_t new_cap = limit / 10 + 10;      /* rough estimate of #primes */
    idx_t *new_primes = (idx_t *)malloc(new_cap * sizeof(idx_t));
    if (new_primes == NULL)
        abort();

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_is_prime[i]) {
            if (cnt == new_cap) {          /* grow array if needed */
                new_cap *= 2;
                new_primes = (idx_t *)realloc(new_primes, new_cap * sizeof(idx_t));
                if (new_primes == NULL)
                    abort();
            }
            new_primes[cnt++] = i;
        }
    }

    /* Replace old data with the newly built sieve */
    free(_is_prime);
    free(_primes);
    _is_prime   = new_is_prime;
    _primes     = new_primes;
    _prime_count = cnt;
    _limit      = limit;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)  /* safety guard – the sequence starts at n = 1 */
        return 0;

    /* Ensure the sieve is initialized to a modest size */
    _ensure_limit(100);

    idx_t isolated_prime = 0;
    idx_t isolated_comp  = 0;
    size_t icount_prime  = 0;
    size_t icount_comp   = 0;
    size_t p_idx         = 0;                /* index into the _primes array */

    /* Search until we have found the n-th isolated prime and the
       n-th isolated composite.  The search is order‑independent. */
    while (icount_prime < n || icount_comp < n) {
        /* If the prime list is exhausted, extend the sieve. */
        if (p_idx >= _prime_count) {
            idx_t new_limit = _limit * 2 + 100;
            if (new_limit <= _limit)
                new_limit = _limit + 100;
            _ensure_limit(new_limit);
            continue;     /* re‑evaluate after extension */
        }

        idx_t p = _primes[p_idx];

        /* Ensure we can check p+2 for the twin property. */
        if (p + 2 > _limit)
            _ensure_limit(p + 2);

        bool twin_lower = (p >= 2) && _is_prime[p - 2];
        bool twin_upper = _is_prime[p + 2];

        /* Isolated prime? (not part of any twin prime pair) */
        if (!twin_lower && !twin_upper) {
            ++icount_prime;
            if (icount_prime == n)
                isolated_prime = p;
        }

        /* Isolated composite: lower element of a twin pair. */
        if (twin_upper) {
            idx_t composite = p + 1;   /* p+1 is always composite */
            ++icount_comp;
            if (icount_comp == n)
                isolated_comp = composite;
        }

        ++p_idx;
    }

    /* Convert to seq_t and return the sum */
    return (seq_t)isolated_prime + (seq_t)isolated_comp;
}
