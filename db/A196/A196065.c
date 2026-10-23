/*
 * A196065  The 1st multiplicative Zagreb index of the rooted tree with
 *          Matula-Goebel number n.
 * https://oeis.org/A196065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1)=0; a(2)=1; * If n = prime(t) with t ≥ 2,
 *            a(n)=a(t)*(1+G(t))^2 / G(t)^2; * If n = r*s (r,s ≥ 2),
 *            a(n)=a(r)*a(s)*G(n)^2 / (G(r)*G(s))^2; * G(m) denotes the number
 *            of prime divisors of m counted with multiplicities * (i.e., the
 *            big‑omega function)
 * Range:     u8 n <= 14, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   bigomega, build_primes, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/build_primes.h"
#include "../utils/build_spf.h"

/* Global cache and helper data (static to keep state between calls) */
static seq_t *cache = NULL;          /* computed a(n) values   */
static bool *computed = NULL;        /* true if cache[n] valid */
static size_t cache_size = 0;        /* current allocated size */

static idx_t *prime_list = NULL;     /* array of primes up to current limit */
static size_t prime_count = 0;       /* number of primes found      */
static idx_t prime_limit = 0;        /* sieve limit used so far     */

static idx_t *spf = NULL;           /* smallest prime factor array */
static idx_t *prime_index = NULL;    /* mapping prime value -> its index t   */
static size_t prime_index_size = 0;  /* size of prime_index array */

/* forward declarations */
static void ensure_cache_size(idx_t n);
static void ensure_primes_and_spf(idx_t limit);
static seq_t compute_a(idx_t n);

/* --- Memory helpers ------------------------------------------- */

static void ensure_cache_size(idx_t n)
{
    if ((size_t)n + 1 <= cache_size) return;

    size_t new_size = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    computed = (bool *)realloc(computed, new_size * sizeof(bool));
    for (size_t i = cache_size; i < new_size; ++i) {
        computed[i] = false;
        cache[i] = 0;
    }
    cache_size = new_size;
}

/* --- Prime / factorisation helpers ---------------------------- */

static void ensure_primes_and_spf(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* free old data */
    free(spf);
    free(prime_list);
    free(prime_index);

    /* compute new spf array */
    idx_t *new_spf = NULL;
    build_spf(limit, &new_spf);
    spf = new_spf;

    /* compute prime list */
    size_t cnt = 0;
    idx_t *new_primes = NULL;
    build_primes(limit, &new_primes, &cnt);
    prime_list = new_primes;
    prime_count = cnt;
    prime_limit = limit;

    /* build mapping prime -> index (1‑based) */
    prime_index = (idx_t *)calloc((size_t)limit + 1, sizeof(idx_t));
    for (size_t i = 0; i < cnt; ++i) {
        prime_index[prime_list[i]] = (idx_t)(i + 1);
    }
    prime_index_size = (size_t)limit + 1;
}

/* --- Core recursive function --------------------------------- */

static seq_t compute_a(idx_t n)
{
    /* base cases */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* ensure cache space */
    ensure_cache_size(n);

    if (computed[n]) return cache[n];

    /* ensure prime & spf data up to n */
    ensure_primes_and_spf(n);

    seq_t result;

    /* prime check via mapping table */
    if (prime_index[n] != 0) {
        /* n is prime, n = prime(t) with t = prime_index[n] */
        idx_t t = prime_index[n];
        /* t==1 corresponds to n==2 which is handled above */
        seq_t a_t = compute_a(t);
        seq_t G_t = bigomega(t);
        seq_t one_plus_G = (seq_t)1 + G_t;
        seq_t num = a_t * (one_plus_G * one_plus_G);
        seq_t den = G_t * G_t;
        result = num / den;
    } else {
        /* composite: use smallest prime factor r */
        idx_t r = spf[n];
        idx_t s = n / r;           /* s >= 2 automatically */

        seq_t a_r = compute_a(r);
        seq_t a_s = compute_a(s);
        seq_t G_n = bigomega(n);
        seq_t G_r = bigomega(r);
        seq_t G_s = bigomega(s);

        seq_t num = a_r * a_s * (G_n * G_n);
        seq_t den = (G_r * G_r) * (G_s * G_s);
        result = num / den;
    }

    cache[n] = result;
    computed[n] = true;
    return result;
}

/* --- Public API entry point --------------------------------- */

seq_t a(idx_t n)
{
    return compute_a(n);
}
