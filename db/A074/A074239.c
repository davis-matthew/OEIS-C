/*
 * A074239  Related to cumulative number of non-twin primes.
 * https://oeis.org/A074239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of gaps > 2 between successive odd primes up to
 *            op(n). The function generates odd primes up to index n and
 *            counts the gaps that exceed 2. This requires an incremental
 *            prime sieve and a small cache of previously computed sequence
 *            values
 * Range:     u8 n <= 322, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- static storage ----------------------------------------------------- */
static uint64_t *primes = NULL;           /* array of primes (including 2)          */
static size_t   primes_size = 0;           /* number of primes stored               */
static size_t   primes_cap   = 0;           /* current capacity of primes array      */

static seq_t   *a_cache    = NULL;         /* cached values of a(n)                 */
static size_t   a_cap      = 0;             /* capacity of the cache                 */
static size_t   a_cache_size = 0;           /* number of a(n) values cached          */

static size_t   current_a_index = 0;        /* highest a(n) value computed so far    */
static uint64_t last_odd_prime   = 0;       /* last odd prime processed (op of index)*/
static seq_t    non_twin_gap_count = 0;     /* current count of gaps > 2             */

/* ------------------------------------------------------------------------ */
static void grow_primes(void)
{
    if (primes_size == primes_cap) {
        primes_cap = primes_cap ? primes_cap * 2 : 8;
        uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
    }
}

static void grow_a_cache(void)
{
    if (a_cache_size == a_cap) {
        a_cap = a_cap ? a_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
    }
}

/* ------------------------------------------------------------------------ */
static void init_storage(void)
{
    if (primes_size != 0) return;          /* already initialised */

    /* prime array starts with 2 and 3 (first odd prime) */
    grow_primes();
    primes[primes_size++] = 2;
    primes[primes_size++] = 3;

    /* cache holds a(0)=0 */
    grow_a_cache();
    a_cache[a_cache_size++] = (seq_t)0;
    current_a_index = 0;
    last_odd_prime = 3;                     /* op(0) */
    non_twin_gap_count = 0;
}

/* ------------------------------------------------------------------------ */
/* Ensure that primes array contains at least target_n + 1 primes
   (primes[0] is 2, so primes[target_n] is the (target_n+1)th prime). */
static void ensure_primes_up_to(idx_t target_n)
{
    while (primes_size <= (size_t)(target_n + 1)) {
        /* Candidate: next odd number after the last prime      */
        uint64_t candidate = primes[primes_size - 1] + 2;
        if (candidate <= 2) candidate = 3;

        for (;;) {
            bool is_prime = true;
            for (size_t i = 0; i < primes_size; ++i) {
                uint64_t p = primes[i];
                if (p * p > candidate) break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) break;
            candidate += 2;               /* skip even numbers */
        }

        grow_primes();
        primes[primes_size++] = candidate;
    }
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_storage();

    if (n < current_a_index)
        return a_cache[n];                 /* already computed */

    /* Generate primes up to the required index */
    ensure_primes_up_to(n);

    /* Incrementally compute a for new indices */
    for (idx_t i = current_a_index + 1; i <= n; ++i) {
        /* op(i) is primes[i+1] because primes[0] = 2, primes[1] = 3 = op(0) */
        uint64_t op_i = primes[(size_t)(i + 1)];

        uint64_t gap = op_i - last_odd_prime;
        if (gap > 2)
            ++non_twin_gap_count;

        last_odd_prime = op_i;

        grow_a_cache();
        a_cache[a_cache_size++] = non_twin_gap_count;
        current_a_index = (size_t)i;
    }

    return a_cache[n];
}
