/*
 * A235646  Primes p such that b=2*p+1 is semiprime, c=2*b+1 is 3-almost prime
 *          and d=2*c+1 is 4-almost prime.
 * https://oeis.org/A235646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _smallest_prime_factor, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Compute the OEIS A235646 sequence:
 *  Primes p such that
 *      b = 2*p + 1         is semiprime,
 *      c = 2*b + 1         is 3‑almost‑prime,
 *      d = 2*c + 1         is 4‑almost‑prime.
 *  The function returns the n‑th term (1‑based indexing).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Utilities from the build environment */
#include "../utils/next_prime.h"          /* uint64_t next_prime(uint64_t start); */
#include "../utils/seq_realloc.h"         /* seq_t *seq_realloc(seq_t*, size_t, size_t); */
#include "../utils/_smallest_prime_factor.h" /* uint64_t _smallest_prime_factor(uint64_t n); */

/* ------------------------------------------------------ */
/*  Helper: count prime factors of a uint64_t number       */
/*  up to a given limit. If the real count exceeds the    */
/*  limit, the function returns a value > limit.          */
/* ------------------------------------------------------ */
static int count_factors_limit(uint64_t n, int limit)
{
    int cnt = 0;
    while (n > 1 && cnt <= limit)
    {
        uint64_t f = _smallest_prime_factor(n);
        if (f == 0)   /* safety check, should never happen for n > 1 */
            break;
        n /= f;
        ++cnt;
        if (cnt > limit)
            return cnt;
    }
    return cnt;
}

/* ------------------------------------------------------ */
/*  static cache for the generated sequence                */
/* ------------------------------------------------------ */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of terms actually stored */
static size_t cache_cap  = 0;   /* allocated capacity */
static uint64_t last_checked_prime = 1;  /* value of the last prime examined */

/* ------------------------------------------------------ */
/*  Main sequence function                                */
/* ------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n = 0 return 0. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Initialise cache once. */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = seq_realloc(NULL, 0, cache_cap);   /* allocate first block */
    }

    /* Generate terms until we have at least n items. */
    while (cache_len < n)
    {
        /* Find the next prime after the last one examined. */
        uint64_t p = next_prime(last_checked_prime + 1);
        last_checked_prime = p;

        /* Compute the derived numbers. */
        uint64_t b = 2 * p + 1;
        uint64_t c = 2 * b + 1;
        uint64_t d = 2 * c + 1;

        /* Apply the factor-count criteria. */
        if (count_factors_limit(b, 2) != 2)
            continue;                     /* b is not semiprime */
        if (count_factors_limit(c, 3) != 3)
            continue;                     /* c is not 3‑almost‑prime */
        if (count_factors_limit(d, 4) != 4)
            continue;                     /* d is not 4‑almost‑prime */

        /* Ensure there is space in the cache. */
        if (cache_len >= cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the prime p in the cache. */
        cache[cache_len++] = (seq_t)p;
    }

    /* Return the nth term (1‑based index). */
    return cache[n - 1];
}
