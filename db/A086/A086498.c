/*
 * A086498  Rearrangement of primes such that every (2n)-th partial sum is a
 *          prime. Every (2n+1)-st term is the smallest prime which has not
 *          been included earlier.
 * https://oeis.org/A086498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 995, u32 n <= 97, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A086498
 *  a(1)=2, a(2)=3, a(3)=5, a(4)=7, a(5)=11, a(6)=13, …
 *  All odd positions contain the next unused prime.
 *  All even positions contain the smallest unused prime such that
 *  the running sum stays prime.  This implementation uses a simple
 *  linear search with a growing list of primes and memoises the
 *  sequence to answer calls in any order.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

/* Prime storage */
static uint64_t *primes = NULL;   /* array of primes                    */
static bool *used     = NULL;     /* used[i] == true  => prime i used   */
static size_t prime_cnt = 0;      /* number of primes stored             */
static size_t prime_cap = 0;      /* current capacity of arrays          */

/* Sequence cache */
static seq_t *cache = NULL;       /* array of computed values            */
static size_t cache_len = 0;      /* number of terms actually computed   */
static size_t cache_cap = 0;      /* current cache capacity              */

/* State for sequence generation */
static seq_t running_sum = 0;     /* sum of already generated terms      */
static size_t next_unused_idx = 0;/* index of first unused prime         */

/* ------------------------------------------------------------------ */
/* Initialise prime list with the first prime.                       */
static void init_primes(void)
{
    if (prime_cnt != 0)
        return;
    prime_cap = 4;
    primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    used   = (bool *)malloc(prime_cap * sizeof(bool));
    primes[0] = 2;
    used[0]   = false;
    prime_cnt = 1;
}

/* ------------------------------------------------------------------ */
/* Grow the prime/used arrays when more primes are needed.           */
static void grow_prime_arrays(void)
{
    if (prime_cnt < prime_cap)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 4;
    uint64_t *newp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    bool *newu   = (bool *)realloc(used,   new_cap * sizeof(bool));
    primes = newp;
    used   = newu;
    prime_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Append the next prime to the list.                                 */
static void add_next_prime(void)
{
    uint64_t last = primes[prime_cnt - 1];
    uint64_t nxt  = next_prime(last);
    grow_prime_arrays();
    primes[prime_cnt] = nxt;
    used[prime_cnt]   = false;
    prime_cnt++;
}

/* ------------------------------------------------------------------ */
/* Find the index of the next unused prime (used[ ]==false).          */
static size_t find_next_unused_prime_index(void)
{
    while (next_unused_idx >= prime_cnt)
        add_next_prime();
    while (used[next_unused_idx])
    {
        next_unused_idx++;
        if (next_unused_idx >= prime_cnt)
            add_next_prime();
    }
    return next_unused_idx;
}

/* ------------------------------------------------------------------ */
/* Ensure the cache can hold at least n entries.                      */
static void ensure_cache(idx_t n)
{
    if (n == 0)
        return;
    if (cache == NULL)
    {
        cache_cap = n > 64 ? n : 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }
    else if ((size_t)n > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n < 1 */

    init_primes();
    ensure_cache(n);

    /* Generate missing terms if needed. */
    for (size_t i = cache_len; i < (size_t)n; ++i)
    {
        size_t idx1 = i + 1;           /* 1‑based index */
        seq_t term = 0;

        if (idx1 % 2 == 1)  /* odd position – next unused prime */
        {
            size_t pi = find_next_unused_prime_index();
            term = (seq_t)primes[pi];
            used[pi] = true;
            /* advance the search index for the next odd position */
            if (pi == next_unused_idx)
                next_unused_idx++;
        }
        else               /* even position – smallest unused prime
                              that keeps the running sum prime */
        {
            size_t pi = 0;
            while (true)
            {
                /* ensure we have enough primes to test */
                while (pi >= prime_cnt)
                    add_next_prime();

                if (used[pi])          /* skip already used primes   */
                {
                    pi++;
                    continue;
                }

                seq_t cand_sum = running_sum + (seq_t)primes[pi];
                if (is_prime_big(cand_sum))
                {
                    term = (seq_t)primes[pi];
                    used[pi] = true;
                    break;
                }
                pi++;
            }
        }

        cache[i] = term;
        running_sum += term;
    }

    cache_len = (size_t)n;   /* now we have computed up to n */
    return cache[(size_t)n - 1];
}
