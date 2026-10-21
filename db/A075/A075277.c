/*
 * A075277  Interprimes (A024675) which are of the form s*prime, s=2.
 * https://oeis.org/A075277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 248, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075277 (Interprimes that are 2×prime)
 *
 * Formula: a(n) is the n‑th integer m such that
 *          m = (p + q)/2 where p and q are consecutive primes,
 *          and m is divisible by 2 with m/2 being prime.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* ---------- Static state for memoisation ---------- */
static idx_t seq_cnt   = 0;      /* number of terms found */
static idx_t seq_cap   = 0;      /* allocated capacity */
static seq_t *seq_arr  = NULL;   /* array of sequence values */

static idx_t  last_prime   = 0;  /* last prime found */
static idx_t  next_candidate = 2;/* next number to test for primality */
static bool   initialized   = false;

/* ---------- Helper: grow sequence array ---------- */
static void grow_seq_arr(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_arr = tmp;
    seq_cap = new_cap;
}

/* ---------- Find the next prime and process pairs ---------- */
static void find_next_prime_and_process(void)
{
    idx_t candidate = next_candidate;

    /* search for the next prime */
    while (!is_prime(candidate)) {
        candidate += (candidate==2 ? 1 : 2);   /* skip even numbers after 2 */
    }

    /* candidate is prime */
    idx_t prime = candidate;

    /* if we have a previous prime, check the consecutive pair */
    if (last_prime != 0) {
        idx_t sum = last_prime + prime;
        if ((sum & 3ULL) == 0) {          /* sum divisible by 4? */
            idx_t r = sum >> 2;           /* r = sum / 4               */
            if (is_prime(r)) {            /* r must be prime            */
                seq_t val = (seq_t)2 * (seq_t)r;
                grow_seq_arr(seq_cnt + 1);
                seq_arr[seq_cnt++] = val;
            }
        }
    }

    /* update state for next iteration */
    last_prime = prime;
    next_candidate = (prime==2) ? 3 : prime + 2;
}

/* ---------- Ensure we have at least n terms ---------- */
static void ensure_n_terms(idx_t n)
{
    if (!initialized) {
        /* first call: state already set to defaults */
        initialized = true;
    }
    while (seq_cnt < n) {
        find_next_prime_and_process();
    }
}

/* ---------- Public function required by the framework ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n<1 */
    ensure_n_terms(n);
    return seq_arr[n-1];
}
