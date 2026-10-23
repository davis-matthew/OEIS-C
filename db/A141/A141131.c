/*
 * A141131  Duplicate of A038873.
 * https://oeis.org/A141131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is the n-th prime p such that p == 2 or p % 8 == 1 or p % 8
 *            == 7 (i.e. primes for which 2 is a quadratic residue modulo p)
 * Range:     i8 n <= 15, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t* primes_all   = NULL;
static size_t primes_all_len  = 0;
static size_t primes_all_cap  = 0;

static seq_t* cache   = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

static idx_t next_candidate = 3; /* start testing odd numbers after 2 */

/* enlarge the array of all primes to at least 'need' capacity */
static void ensure_primes_cap(size_t need)
{
    if (need <= primes_all_cap) return;
    size_t newcap = primes_all_cap ? primes_all_cap * 2 : 32;
    while (newcap < need) newcap <<= 1;
    seq_t *newp = (seq_t *)realloc(primes_all, newcap * sizeof(seq_t));
    primes_all   = newp;
    primes_all_cap = newcap;
}

/* enlarge the cache of qualifying primes to at least 'need' capacity */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 32;
    while (newcap < need) newcap <<= 1;
    seq_t *newc = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache  = newc;
    cache_cap = newcap;
}

/* initialize our global state lazily on the first call */
static void init_globals(void)
{
    if (primes_all_len != 0) return; /* already initialized */

    /* prime 2 */
    ensure_primes_cap(1);
    primes_all[0] = (seq_t)2;
    primes_all_len = 1;

    /* it satisfies the residue condition */
    ensure_cache_cap(1);
    cache[0] = (seq_t)2;
    cache_len = 1;
}

/* test if candidate is a prime using trial division by previously found primes */
static int is_candidate_prime(idx_t candidate)
{
    for (size_t i = 0; i < primes_all_len; ++i) {
        idx_t p = (idx_t)primes_all[i];
        if (p > candidate / p) break;           /* stop once p^2 > candidate */
        if (candidate % p == 0) return 0;        /* composite */
    }
    return 1; /* prime */
}

/* public function requested by the problem statement */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for 0, doesn't matter per spec */

    init_globals();

    /* generate more primes until we have enough qualifying primes */
    while (cache_len < (size_t)n) {
        idx_t cand = next_candidate;
        next_candidate += 2;  /* only test odd numbers */

        if (is_candidate_prime(cand)) {
            /* store the new prime */
            ensure_primes_cap(primes_all_len + 1);
            primes_all[primes_all_len++] = cand;

            /* if it satisfies the residue condition, add to cache */
            if (cand == 2 || cand % 8 == 1 || cand % 8 == 7) {
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = cand;
            }
        }
    }

    return cache[n - 1];
}
