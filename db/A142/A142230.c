/*
 * A142230  Primes congruent to 33 mod 41.
 * https://oeis.org/A142230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 153, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th prime p such that p ≡ 33 (mod 41)
 * The algorithm generates primes of the form 41*k + 33 and
 * tests each candidate for primality using the provided is_prime().
 * Results are cached so that calls with arbitrary n are efficient.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t   *primes_residue = NULL;   /* primes ≡ 33 (mod 41) */
static size_t   primes_res_cap  = 0;
static size_t   primes_res_cnt  = 0;
static uint64_t next_k          = 0;     /* next k for candidate 33 + 41*k */

/* Ensure the array has space for at least 'need' elements */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : *cap * 2;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr   = tmp;
        *cap   = new_cap;
    }
}

/* Generate primes of the required residue until we have at least n_needed */
static void generate_upto(size_t n_needed)
{
    if (primes_res_cnt >= n_needed) return;

    while (primes_res_cnt < n_needed) {
        uint64_t candidate = 33ULL + 41ULL * next_k;
        ++next_k;   /* advance to next residue candidate */

        if (is_prime((idx_t)candidate)) {
            grow_array(&primes_residue, &primes_res_cap, primes_res_cnt + 1);
            primes_residue[primes_res_cnt++] = (seq_t)candidate;
        }
    }
}

/* Sequence function requested by the framework */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out of range, but not part of OEIS offset */

    generate_upto((size_t)n);
    return primes_residue[(size_t)(n - 1)];
}
