/*
 * A140470  Numbers n such that p+1 divides n for every prime p that divides
 *          n.
 * https://oeis.org/A140470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 702, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140470: Numbers n such that p+1 divides n for every prime p that divides n. */
/* The function enumerates integers in order and tests the primality condition using
   a precomputed list of primes up to 10⁶.  Results are cached so that subsequent
   calls are answered immediately. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PRIME_LIMIT 1000000U   /* upper bound for prime sieve (larger values need more memory) */

static uint32_t primes[80000];        /* buffer large enough for primes <= 10⁶ */
static size_t   primes_cnt = 0;      /* number of primes stored */

/*------------------------------------------------------------------*/
/* prime sieve – executed once at first call to a()                  */
/*------------------------------------------------------------------*/
static void init_primes(void)
{
    static bool done = false;
    if (done) return;

    char *sieve = (char*)malloc((PRIME_LIMIT + 1) * sizeof(char));
    if (!sieve) exit(1);                     /* fatal if out of memory */
    memset(sieve, 1, PRIME_LIMIT + 1);
    sieve[0] = sieve[1] = 0;
    idx_t i;
    for (i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;
        }
    }
    for (i = 2; i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) primes[primes_cnt++] = (uint32_t)i;
    }
    free(sieve);
    done = true;
}

/*------------------------------------------------------------------*/
/* check whether m satisfies the property                          */
/*------------------------------------------------------------------*/
static bool is_good(idx_t m)
{
    if (m == 1) return true;          /* special case: 1 is in the sequence */

    idx_t tmp = m;
    size_t k;
    for (k = 0; k < primes_cnt; ++k) {
        idx_t p = primes[k];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            if (m % (p + 1) != 0) return false;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {                    /* remaining prime factor */
        if (m % (tmp + 1) != 0) return false;
    }
    return true;
}

/*------------------------------------------------------------------*/
/* dynamic cache for sequence values                               */
/*------------------------------------------------------------------*/
static seq_t *cache = NULL;            /* stored sequence values */
static size_t cache_capacity = 0;
static size_t cache_size = 0;
static idx_t next_checked = 1;         /* next integer to test */

/* enlarge cache when needed */
static void cache_push(seq_t val)
{
    if (cache_size == cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_capacity = new_cap;
    }
    cache[cache_size++] = val;
}

/*------------------------------------------------------------------*/
/* main function                                                   */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined for safety, though n ≥ 1 in use */

    static bool init_done = false;
    if (!init_done) {
        init_primes();             /* prime list first‑time */
        cache = NULL;
        cache_capacity = 0;
        cache_size = 0;
        next_checked = 1;
        init_done = true;
    }

    /* generate further terms until we have at least n entries */
    while ((idx_t)cache_size < n) {
        idx_t candidate = next_checked++;
        if (is_good(candidate))
            cache_push((seq_t)candidate);
    }

    return cache[n - 1];
}
