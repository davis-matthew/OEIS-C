/*
 * A045337  Primes congruent to {1, 2, 3, 4} (mod 7).
 * https://oeis.org/A045337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A045337: primes congruent to 1,2,3,4 (mod 7).
   The function generates all primes in order and keeps those with the
   required residues, memoising the results so that any call order is fine. */

#include <stdlib.h>
#include <stdbool.h>

/* Static arrays to store found primes and the sequence values. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;
static uint64_t last_checked_prime = 1;

static seq_t *seq_vals = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Increase the capacity of a uint64_t array when needed. */
static void grow_uint64_array(uint64_t **arr, size_t *cap)
{
    if (*cap == 0) *cap = 64;
    else *cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(*arr, *cap * sizeof(**arr));
    if (!tmp) abort();
    *arr = tmp;
}

/* Increase the capacity of a seq_t array when needed. */
static void grow_seq_array(seq_t **arr, size_t *cap)
{
    if (*cap == 0) *cap = 64;
    else *cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, *cap * sizeof(**arr));
    if (!tmp) abort();
    *arr = tmp;
}

/* Test whether cand is a prime using the primes already found. */
static bool is_prime(uint64_t cand)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > cand / p) break;  /* avoid overflow */
        if (cand % p == 0) return false;
    }
    return true;
}

/* Ensure that seq_vals contains at least n elements (1‑based). */
static void ensure_up_to(idx_t n)
{
    while (seq_len < (size_t)n) {
        /* Find the next prime candidate. */
        uint64_t cand = last_checked_prime + 1;
        if (cand <= 2) cand = 2;
        else if ((cand & 1U) == 0) cand++;                 /* make odd   */

        while (!is_prime(cand)) cand += 2;                   /* skip even */

        /* Store the new prime. */
        if (prime_cnt >= prime_cap) grow_uint64_array(&prime_list, &prime_cap);
        prime_list[prime_cnt++] = cand;
        last_checked_prime = cand;

        /* Append if residue is 1,2,3,4 modulo 7. */
        uint64_t r = cand % 7U;
        if (r != 0 && r != 5 && r != 6) {
            if (seq_len >= seq_cap) grow_seq_array(&seq_vals, &seq_cap);
            seq_vals[seq_len++] = (seq_t)cand;
        }
    }
}

/* Public function: return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety for non‑offset calls */
    ensure_up_to(n);
    return seq_vals[n - 1];
}
