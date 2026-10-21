/*
 * A083715  (greatest prime <= n) mod (greatest prime factor of n).
 * https://oeis.org/A083715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (greatest prime <= n) mod (greatest prime factor of n)
 *            Offset: 1 (a(1) is the first listed term)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* -------------------------------------------------------------------- */
/*  Static state for prime generation                                 */
/* -------------------------------------------------------------------- */
static uint64_t *primes = NULL;      /* array of primes up to current_limit */
static size_t   prime_count = 0;     /* number of primes stored          */
static uint64_t current_limit = 0;   /* maximum n for which primes exist */

/* -------------------------------------------------------------------- */
/*  Forward declarations                                               */
/* -------------------------------------------------------------------- */
static void recompute_primes(uint64_t limit);
static void ensure_primes_up_to(uint64_t limit);
static uint64_t greatest_prime_leq(uint64_t n);
static uint64_t largest_prime_factor(uint64_t n);

/* -------------------------------------------------------------------- */
/*  Recompute all primes up to 'limit' (inclusive) using sieve.        */
/* -------------------------------------------------------------------- */
static void recompute_primes(uint64_t limit)
{
    /* Free any previously allocated prime list */
    if (primes != NULL) free(primes);
    primes = NULL;
    prime_count = 0;
    current_limit = limit;

    if (limit < 2) return;        /* no primes needed */

    /* Allocate a boolean array for composite markers. */
    size_t sz_iscomp = (size_t)limit + 1;
    bool *is_comp = (bool *)calloc(sz_iscomp, sizeof(bool));
    if (!is_comp) return;         /* allocation failure: abort */

    /* Estimate initial space for prime array. */
    size_t primes_arr_size = limit / 10 + 1;   /* heuristic */
    primes = (uint64_t *)malloc(primes_arr_size * sizeof(uint64_t));
    if (!primes) { free(is_comp); return; }

    size_t p_cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (!is_comp[i]) {                      /* i is prime */
            if (p_cnt >= primes_arr_size) {     /* grow array if needed */
                primes_arr_size *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_arr_size * sizeof(uint64_t));
                if (!tmp) { free(is_comp); free(primes); primes = NULL; return; }
                primes = tmp;
            }
            primes[p_cnt++] = (uint64_t)i;

            /* Mark multiples only if i*i does not overflow and <= limit */
            if (i <= (size_t)limit / i) {
                size_t start = i * i;
                for (size_t j = start; j <= (size_t)limit; j += i)
                    is_comp[j] = true;
            }
        }
    }

    free(is_comp);
    prime_count = p_cnt;
}

/* -------------------------------------------------------------------- */
/*  Ensure that the prime list contains all primes <= 'limit'.          */
/* -------------------------------------------------------------------- */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= current_limit) return;
    recompute_primes(limit);
}

/* -------------------------------------------------------------------- */
/*  Return the greatest prime <= n (for n >= 2).                        */
/* -------------------------------------------------------------------- */
static uint64_t greatest_prime_leq(uint64_t n)
{
    if (n < 2) return 0;
    size_t lo = 0, hi = prime_count;   /* hi is exclusive */
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo == 0) return 0;
    return primes[lo - 1];
}

/* -------------------------------------------------------------------- */
/*  Return the largest prime factor of n (for n >= 2).                  */
/* -------------------------------------------------------------------- */
static uint64_t largest_prime_factor(uint64_t n)
{
    uint64_t rem = n;
    uint64_t largest = 0;
    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= rem; ++i) {
        uint64_t p = primes[i];
        if (rem % p == 0) {
            largest = p;
            while (rem % p == 0) rem /= p;
        }
    }
    if (rem > 1) largest = rem;            /* remaining part is prime */
    return largest;
}

/* -------------------------------------------------------------------- */
/*  Main sequence function                                            */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined, return 0 for safety */
    if (n == 1) return 0;         /* a(1) = 0 by definition */

    /* Prepare primes up to n */
    ensure_primes_up_to((uint64_t)n);

    uint64_t gp  = greatest_prime_leq((uint64_t)n);          /* greatest prime <= n */
    if (gp == 0) return 0;            /* should not happen for n >= 2 */

    uint64_t gpf = largest_prime_factor((uint64_t)n);        /* largest prime factor of n */
    if (gpf == 0) return 0;            /* should not happen */

    seq_t result = (seq_t)(gp % gpf);
    return result;
}
