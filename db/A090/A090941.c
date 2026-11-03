/*
 * A090941  Primes arising as the arithmetic means of the terms of A090940.
 * https://oeis.org/A090941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic mean of the first n terms of A090940, where
 *            A090940 is defined as the sequence of primes p_k such that the
 *            arithmetic mean of the first k terms is a prime
 * Range:     u8 n <= 8, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/isqrt.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* static state for the sequence */
static idx_t generated = 0;                      /* number of terms generated */
static seq_t *prefix_sums = NULL;                /* prefix sums S[0..generated] */
static size_t prefix_cap = 0;                    /* capacity of prefix_sums array */
static uint64_t *chosen_primes = NULL;           /* primes that have already been chosen */
static size_t chosen_cap = 0, chosen_len = 0;     /* capacity and length of chosen_primes */
static uint64_t *all_primes = NULL;              /* list of all primes discovered so far */
static size_t all_cap = 0, all_len = 0;          /* capacity and length of all_primes */
static bool *used_arr = NULL;                    /* bit‑set for checking whether a prime
                                                    has already been used */
static size_t used_cap = 0;                      /* capacity of used_arr (in elements) */
static uint64_t next_generate_candidate = 3;     /* next integer to test for primality */

/* ensure that used_arr can index up to value */
static void ensure_used_cap(uint64_t value)
{
    if (value < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 256;
    while (new_cap <= value) new_cap *= 2;
    bool *new_arr = (bool *)calloc(new_cap, 1);
    if (used_arr) {
        memcpy(new_arr, used_arr, used_cap);
        free(used_arr);
    }
    used_arr = new_arr;
    used_cap = new_cap;
}
static inline bool used_uint64(uint64_t v) { return v < used_cap && used_arr[v]; }
static inline void mark_used(uint64_t v) { ensure_used_cap(v); used_arr[v] = true; }

/* grow an array of uint64_t on demand */
static void grow_uint64_array(uint64_t **arr, size_t *cap, size_t *len)
{
    if (*len < *cap) return;
    *cap = *cap ? *cap * 2 : 16;
    *arr = (uint64_t *)realloc(*arr, *cap * sizeof(uint64_t));
}

/* generate the next prime (greater than last discovered) and push it to all_primes */
static void add_next_prime(void)
{
    while (1) {
        /* skip 2 – this sequence never uses the prime 2 */
        if (next_generate_candidate % 2 == 0) {
            if (next_generate_candidate == 2)
                next_generate_candidate = 3;
            else {
                next_generate_candidate++;
                continue;
            }
        }

        uint64_t num = next_generate_candidate;
        bool is_prime = true;
        uint64_t limit = isqrt(num);
        for (size_t j = 0; j < all_len; ++j) {
            uint64_t p = all_primes[j];
            if (p > limit) break;
            if (num % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            grow_uint64_array(&all_primes, &all_cap, &all_len);
            all_primes[all_len++] = num;
        }
        next_generate_candidate++;
        if (is_prime) break;
    }
}

/* initialize the static storage once */
static void seq_init(void)
{
    if (prefix_sums) return;                /* already initialized */
    prefix_cap = 16;
    prefix_sums = mem_new(seq_t, prefix_cap);
    prefix_sums[0] = (seq_t)0;              /* S0 = 0 */
    chosen_cap = 16;
    chosen_primes = (uint64_t *)calloc(chosen_cap, sizeof(uint64_t));
    all_cap = 16;
    all_primes = (uint64_t *)calloc(all_cap, sizeof(uint64_t));
    next_generate_candidate = 3;             /* start with the first odd prime */
}

/* main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, but harmless */
    seq_init();

    /* generate terms up to n */
    while (generated < n) {
        idx_t idx = generated + 1;            /* current desired index */
        seq_t prev_sum = prefix_sums[generated];
        seq_t new_sum = 0;
        uint64_t chosen_prime = 0;

        /* search over all known primes, generating more as needed */
        for (size_t i = 0;; ++i) {
            if (i == all_len) add_next_prime();   /* extend prime list */

            uint64_t cand = all_primes[i];
            if (used_uint64(cand)) continue;       /* already used as a term */

            new_sum = prev_sum + (seq_t)cand;
            if (new_sum % (seq_t)idx != 0) continue;

            seq_t avg = new_sum / (seq_t)idx;
            if (!is_prime_big(avg)) continue;

            chosen_prime = cand;                   /* accept this prime */
            break;
        }

        /* record the new term and its prefix sum */
        grow_uint64_array(&chosen_primes, &chosen_cap, &chosen_len);
        chosen_primes[chosen_len++] = chosen_prime;

        if (generated + 1 >= prefix_cap) {
            prefix_cap = prefix_cap ? prefix_cap * 2 : 16;
            prefix_sums = mem_resize(seq_t, prefix_sums, prefix_cap);
        }
        prefix_sums[generated + 1] = new_sum;

        mark_used(chosen_prime);
        ++generated;
    }

    return prefix_sums[n] / (seq_t)n;          /* arithmetic mean of first n terms */
}
