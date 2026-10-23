/*
 * A106009  Primes p such that 20*p - 3 and 20*p + 3 are both primes.
 * https://oeis.org/A106009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 440, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search for primes p such that 20p-3 and 20p+3 are also prime. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

#ifdef __cplusplus
#include <vector>
#endif

/* static state --------------------------------------------------------- */
static bool init_done   = false;            /* has the list been initialised? */
static uint64_t candidate = 2;              /* next integer to test */

#ifdef __cplusplus
/* C++ build: use std::vector for automatic construction of seq_t objects */
static std::vector<seq_t> good_primes;
static void init_sequence(void)
{
    if (init_done) return;
    good_primes.reserve(1024);   /* start with a modest capacity */
    init_done = true;
}

static inline size_t good_cnt(void) { return good_primes.size(); }
static inline void add_good(seq_t val) { good_primes.push_back(val); }
static inline seq_t get_good(idx_t i) { return good_primes[i-1]; }

#else
/* C build: use malloc/realloc.  The type seq_t is a plain unsigned integer so
 * storage can be managed with the standard library. */
static seq_t *good_primes = NULL;
static size_t good_cap = 0;
static size_t good_cnt_value = 0;

static void init_sequence(void)
{
    if (init_done) return;
    good_cap   = 16;
    good_primes = (seq_t *)malloc(good_cap * sizeof(seq_t));
    if (!good_primes) exit(EXIT_FAILURE);
    init_done = true;
}

static inline size_t good_cnt(void) { return good_cnt_value; }
static inline void add_good(seq_t val)
{
    if (good_cnt_value >= good_cap) {
        good_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(good_primes, good_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        good_primes = tmp;
    }
    good_primes[good_cnt_value++] = val;
}
static inline seq_t get_good(idx_t i) { return good_primes[i-1]; }
#endif

seq_t a(idx_t n)
{
    init_sequence();

    /* Generate primes until we have at least n elements. */
    while (good_cnt() < (size_t)n) {
        uint64_t p = candidate;
        if (is_prime(p) &&
            is_prime(20*p - 3ULL) &&
            is_prime(20*p + 3ULL)) {
            add_good((seq_t)p);
        }

        /* Advance to the next candidate: after 2 skip even numbers. */
        if (candidate == 2ULL)
            candidate = 3ULL;
        else
            candidate += 2ULL;
    }

    return get_good(n);
}
