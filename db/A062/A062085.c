/*
 * A062085  Squarefree numbers with all even digits.
 * https://oeis.org/A062085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Returns the n-th squarefree number whose decimal digits are all even
    (digits 0,2,4,6,8).  The sequence is defined by searching for numbers
    that satisfy these two conditions.  The routine uses memoization so
    any order of calls is fine.  It runs in O(m) time where m is the value
    of the largest number required (≈1 000 000 for n≈10 000).  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Prime sieve (initialised once)                                    */
/* ------------------------------------------------------------------ */
static uint64_t *primes = NULL;   /* array of primes */
static size_t   primes_cnt = 0;   /* number of primes stored */
static uint64_t sieve_limit = 0;  /* max prime that has been sieved */

static void init_primes(uint64_t limit)
{
    if (limit <= sieve_limit) return;          /* already done */

    /*--- Simple Eratosthenes sieve ---------------------------------*/
    uint8_t *is_composite = (uint8_t*)calloc(limit + 1, 1);
    if (!is_composite) abort();

    size_t est = limit / 10 + 1;                 /* rough estimate of prime count */
    primes = (uint64_t*)realloc(primes, est * sizeof(uint64_t));
    primes_cnt = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            if (primes_cnt == est) {            /* expand storage if needed */
                est *= 2;
                primes = (uint64_t*)realloc(primes, est * sizeof(uint64_t));
            }
            primes[primes_cnt++] = i;

            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
            }
        }
    }

    free(is_composite);
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Helper predicates                                                   */
/* ------------------------------------------------------------------ */
static inline bool all_even_digits_uint64(uint64_t x)
{
    while (x) {
        uint8_t d = x % 10;
        if (d & 1) return false;     /* odd digit found */
        x /= 10;
    }
    return true;
}

static inline bool is_squarefree_uint64(uint64_t x)
{
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t sq = p * p;
        if (sq > x) break;
        if (x % sq == 0) return false;      /* divisible by a square */
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Memoised sequence generation                                       */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL;   /* array of already found terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* capacity of cache array   */
static uint64_t next_to_check = 2; /* smallest integer not yet examined */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; guard */

    /* initialise once */
    if (!cache) {
        cache_cap = 256;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_to_check = 2;
        init_primes(2000000);      /* enough for n up to ~10 000  */
    }

    /* generate terms until the nth one is ready */
    while (cache_len < n) {
        uint64_t cand = next_to_check++;

        if (!all_even_digits_uint64(cand)) continue;
        if (!is_squarefree_uint64(cand))   continue;

        if (cache_len == cache_cap) {          /* grow cache */
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len++] = (seq_t)cand;      /* store term */
    }

    return cache[n - 1];
}
