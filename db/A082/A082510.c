/*
 * A082510  Differences of consecutive primes being divisible by 6 in order of
 *          their appearance in A001223: terms not divisible by 6 are omitted
 *          from A001223.
 * https://oeis.org/A082510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the difference between consecutive primes that is
 *            divisible by 6 (i.e. p_{k+1} - p_k when p_{k+1} - p_k ≡ 0 (mod
 *            6))
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
/* (seq_t and idx_t are supplied by the build system.) */

/* ------------------------------------------------------------------ */
/*  Prime sieve helpers (trial‑division with a growing list of primes) */
/* ------------------------------------------------------------------ */
static idx_t *primes       = NULL; /* list of primes found so far      */
static idx_t primes_len    = 0;     /* current number of stored primes  */
static idx_t primes_cap    = 0;     /* array capacity                    */
static idx_t last_prime    = 0;     /* most recently found prime          */
static idx_t candidate     = 0;     /* next odd number to test            */
static int   primes_ready  = 0;     /* non‑zero after first initialisation */

static void grow_primes(idx_t new_cap)
{
    idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes = tmp;
    primes_cap = new_cap;
}

static int is_prime_check(idx_t m)
{
    if (m < 2) return 0;
    for (idx_t i = 0; i < primes_len; ++i) {
        idx_t p = primes[i];
        /* avoid overflow in p*p by using division */
        if (p > m / p) break;
        if (m % p == 0) return 0;
    }
    return 1;
}

static void ensure_prime_state(void)
{
    if (primes_ready) return;
    primes_cap = 64;
    primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    if (!primes) exit(EXIT_FAILURE);
    primes_len = 0;
    primes[primes_len++] = 2;   /* first prime */
    last_prime = 2;
    candidate = 3;              /* next odd number to test */
    primes_ready = 1;
}

/* ------------------------------------------------------------------ */
/*  Gap cache (only gaps divisible by 6)                              */
/* ------------------------------------------------------------------ */
static seq_t *gap_cache    = NULL;   /* cached gaps divisible by 6 */
static idx_t  cache_len    = 0;      /* number of cached gaps       */
static idx_t  cache_cap    = 0;      /* allocated capacity of cache */

static void grow_cache(idx_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(gap_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    gap_cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n<1 is undefined but will not be
       requested by the tests. */
    if (n == 0) return 0; /* defensive guard */

    ensure_prime_state();

    if (cache_cap == 0) {          /* initialise cache once */
        cache_cap = 64;
        gap_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!gap_cache) exit(EXIT_FAILURE);
        cache_len = 0;
    }

    /* Generate primes and record gaps until we have at least n cache
       entries. */
    while (cache_len < n) {
        /* Find the next prime.  We skip even numbers. */
        while (!is_prime_check(candidate)) {
            candidate += 2;
        }

        /* Store the prime. */
        if (primes_len == primes_cap) {
            grow_primes(primes_cap << 1);   /* double capacity */
        }
        primes[primes_len++] = candidate;

        /* Compute the gap from the previous prime. */
        idx_t gap = candidate - last_prime;

        /* Store the gap if it is divisible by 6. */
        if (gap % 6 == 0) {
            if (cache_len == cache_cap) {
                grow_cache(cache_cap << 1);  /* double capacity */
            }
            gap_cache[cache_len++] = (seq_t)gap;
        }

        last_prime = candidate;
        candidate += 2;   /* next odd number to test */
    }

    /* Return the n‑th cached gap (1‑based indexing). */
    return gap_cache[n - 1];
}
