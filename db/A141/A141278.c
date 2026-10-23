/*
 * A141278  Clusters of consecutive composites in A141089.
 * https://oeis.org/A141278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th term of OEIS A141278 * (clusters of consecutive
 *            composites in A141089). * The implementation generates the
 *            sequence on demand, * storing partial sums/products of all
 *            composites and * maintaining the list of eligible numbers
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   grow_int_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_int_arr.h"

/* ------------------------------------------------------------------ */
/* Pointers to global data structures (static linkage)                */
/* ------------------------------------------------------------------ */
static uint32_t  *spf        = NULL;   /* smallest prime factor for each number   */
static uint32_t  *prime_exp  = NULL;   /* exponent of each prime in product       */
static int       *primes     = NULL;   /* list of primes up to current limit      */
static size_t       primes_cnt   = 0;
static size_t       primes_cap   = 0;
static uint32_t     sieve_limit  = 0;

static seq_t  *accepted   = NULL;   /* list of A141089 terms (composites accepted) */
static size_t   accepted_sz = 0;
static size_t   accepted_cap = 0;

static seq_t  *cluster    = NULL;   /* list of A141278 terms (consecutive pairs)  */
static size_t   cluster_sz  = 0;
static size_t   cluster_cap = 0;

static seq_t   running_sum = 0;      /* partial sum of all composites up to current number */
static uint32_t next_int   = 4;      /* next integer to test as a composite            */

static bool    data_ready = false;

/* ------------------------------------------------------------------ */
/* Utility array growth helpers                                      */
/* ------------------------------------------------------------------ */
static void grow_seq_t_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 32 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Sieve and factorisation helpers                                   */
/* ------------------------------------------------------------------ */
static void extend_sieve(uint32_t new_limit)
{
    uint32_t old_limit = sieve_limit;

    /* Allocate new tables */
    uint32_t *spf_new = (uint32_t*)calloc(new_limit + 1, sizeof(uint32_t));
    uint32_t *prime_exp_new = (uint32_t*)calloc(new_limit + 1, sizeof(uint32_t));

    /* Prime list capacity estimate */
    size_t primes_new_cap = new_limit / 10 + 32;
    int *primes_new = (int*)malloc(primes_new_cap * sizeof(int));
    size_t primes_new_cnt = 0;

    /* Sieve loop */
    for (uint32_t i = 2; i <= new_limit; ++i) {
        if (spf_new[i] == 0) {
            spf_new[i] = i;
            /* store prime */
            if (primes_new_cnt == primes_new_cap) {
                primes_new_cap <<= 1;
                primes_new = (int*)realloc(primes_new, primes_new_cap * sizeof(int));
            }
            primes_new[primes_new_cnt++] = (int)i;
        }
        for (size_t j = 0; j < primes_new_cnt; ++j) {
            uint32_t p = primes_new[j];
            uint64_t v = (uint64_t)i * p;
            if (v > new_limit) break;
            spf_new[(uint32_t)v] = p;
            if (p == spf_new[i]) break;
        }
    }

    /* Preserve existing exponents up to old_limit */
    if (old_limit > 0 && prime_exp != NULL) {
        for (uint32_t i = 0; i <= old_limit && i <= new_limit; ++i)
            prime_exp_new[i] = prime_exp[i];
    }

    /* Free old structures and replace */
    free(spf);
    free(prime_exp);
    free(primes);

    spf      = spf_new;
    prime_exp = prime_exp_new;
    primes   = primes_new;
    primes_cnt   = primes_new_cnt;
    primes_cap   = primes_new_cap;
    sieve_limit  = new_limit;
}

/* Add prime factor exponents of a composite number (n) to product */
static void add_composite_to_exp(uint32_t n)
{
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t cnt = 0;
        while (n % p == 0) { n /= p; ++cnt; }
        prime_exp[p] += cnt;
    }
}

/* Test whether running_sum divides the product of all composites so far */
static bool sum_divides_product(seq_t sum)
{
    seq_t tmp = sum;
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint32_t p = primes[idx];
        /* stop if p^2 > tmp */
        if ((seq_t)p * (seq_t)p > tmp) break;
        if (tmp % p == 0) {
            uint32_t cnt = 0;
            while (tmp % p == 0) { tmp /= p; ++cnt; }
            if (prime_exp[p] < cnt) return false;
        }
    }
    if (tmp > 1) { /* tmp is a prime factor > sqrt(original tmp) */
        if (tmp > sieve_limit) return false;          /* prime > sieve_limit not in product */
        if (prime_exp[(uint32_t)tmp] == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Ensure that at least 'required' accepted terms (A141089) are found */
/* ------------------------------------------------------------------ */
static void ensure_accepted_terms(size_t required)
{
    while (accepted_sz < required) {
        while (true) {
            /* Extend sieve if needed */
            if (next_int > sieve_limit) extend_sieve(next_int * 2);

            /* Skip primes */
            if (spf[(uint32_t)next_int] == (uint32_t)next_int) {
                ++next_int;
                continue;
            }

            uint32_t candidate = (uint32_t)next_int++;
            running_sum += (seq_t)candidate;
            add_composite_to_exp(candidate);

            if (sum_divides_product(running_sum)) {
                grow_seq_t_array(&accepted, &accepted_cap, accepted_sz + 1);
                accepted[accepted_sz++] = (seq_t)candidate;
                break; /* next required term */
            }
            /* else continue scanning */
        }
    }
}

/* ------------------------------------------------------------------ */
/* Recompute the cluster subsequence (A141278) from accepted terms    */
/* ------------------------------------------------------------------ */
static void recompute_cluster(void)
{
    grow_seq_t_array(&cluster, &cluster_cap, accepted_sz);
    cluster_sz = 0;
    for (size_t i = 0; i < accepted_sz; ++i) {
        bool has_prev = (i > 0) && (accepted[i] == accepted[i - 1] + (seq_t)1);
        bool has_next = (i + 1 < accepted_sz) && (accepted[i] + (seq_t)1 == accepted[i + 1]);
        if (has_prev || has_next) {
            cluster[cluster_sz++] = accepted[i];
        }
    }
}

/* ------------------------------------------------------------------ */
/* Initialise global data on first call                               */
/* ------------------------------------------------------------------ */
static void init_data(void)
{
    if (data_ready) return;
    /* Initialise with a sieve up to 2 million – enough for most early terms */
    extend_sieve(2000000);
    data_ready = true;
}

/* ------------------------------------------------------------------ */
/* Public entry point: a(n)                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!data_ready) init_data();

    while (cluster_sz < (size_t)n) {
        /* Ensure at least one more accepted term so that recomputation can
           possibly produce a new cluster element. */
        ensure_accepted_terms(accepted_sz + 1);
        recompute_cluster();
    }
    return cluster[n - 1];
}
