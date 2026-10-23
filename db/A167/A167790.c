/*
 * A167790  a(n) is the index k of k-th prime prime(k) in the smallest sum
 *          s(k)=2+3+...+prime(k)=t*prime(n) of first k primes where t is a
 *          true divisor and first occurrence of factor prime(n) (n=1,2,3,...)
 * https://oeis.org/A167790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k such that sum of first k primes is divisible by
 *            prime(n) with quotient > 1 (t > 1)
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

/* ----- global structures ----- */
static uint32_t *global_primes = NULL;
static size_t primes_count = 0;
static size_t primes_cap = 0;

static seq_t *global_sums = NULL;
static size_t sums_count = 0;
static size_t sums_cap = 0;

static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;

/* ----- helper to grow arrays ----- */
static void ensure_primes_capacity(void)
{
    if (primes_count == primes_cap) {
        size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
        uint32_t *new_arr = (uint32_t *)realloc(global_primes,
                               new_cap * sizeof(uint32_t));
        if (!new_arr) abort();
        global_primes = new_arr;
        primes_cap = new_cap;
    }
}

static void ensure_sums_capacity(void)
{
    if (sums_count == sums_cap) {
        size_t new_cap = (sums_cap == 0) ? 16 : sums_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(global_sums,
                            new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        global_sums = new_arr;
        sums_cap = new_cap;
    }
}

static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= a_cache_cap) return;
    size_t cap = (a_cache_cap == 0) ? 16 : a_cache_cap;
    while (cap < new_cap) cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(a_cache, cap * sizeof(seq_t));
    if (!new_arr) abort();
    /* initialize new part to 0 */
    memset((void *)(new_arr + a_cache_cap), 0,
           (cap - a_cache_cap) * sizeof(seq_t));
    a_cache = new_arr;
    a_cache_cap = cap;
}

/* ----- generate next prime and extend sums ----- */
static void add_next_prime(void)
{
    uint32_t cand = (primes_count == 0) ? 2 : global_primes[primes_count - 1] + 1;
    while (!isPrimeTrial((seq_t)cand)) ++cand;

    /* add prime */
    ensure_primes_capacity();
    global_primes[primes_count++] = cand;

    /* add cumulative sum */
    seq_t new_sum;
    if (primes_count == 1) new_sum = (seq_t)cand;
    else new_sum = global_sums[sums_count - 1] + (seq_t)cand;

    ensure_sums_capacity();
    global_sums[sums_count++] = new_sum;
}

/* ----- ensure we have at least 'need' primes ----- */
static void ensure_primes_up_to_index(idx_t need)
{
    while (primes_count < (size_t)need)
        add_next_prime();
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* n>=1 by spec, but guard */

    /* cache */
    ensure_cache_capacity((size_t)n);
    if (a_cache[n - 1] != 0) return a_cache[n - 1];

    /* make sure the nth prime is available */
    ensure_primes_up_to_index(n);
    uint32_t prime_n = global_primes[n - 1];

    idx_t k = 1;
    for (;;) {
        /* make sure we have at least k primes and sums */
        ensure_primes_up_to_index(k);
        seq_t sum_k = global_sums[k - 1];
        if ((sum_k % (seq_t)prime_n) == 0) {
            if (sum_k / (seq_t)prime_n > (seq_t)1) {
                a_cache[n - 1] = (seq_t)k;
                return a_cache[n - 1];
            }
        }
        ++k;
    }
}
