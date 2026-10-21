/*
 * A059168  Primes in which digits alternately rise and fall (or vice versa);
 *          sometimes called undulating primes.
 * https://oeis.org/A059168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1578, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_undul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059168: primes whose decimal digits alternate up and down (undulating primes). */
/* Computation: search for primes in increasing order and collect those whose decimal
   digits alternate.  The sequence is returned for any index n ≥ 1. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_undul.h"

/* Allocate a new array of seq_t with the given capacity. */
static seq_t *alloc_seq(size_t cap)
{
    seq_t *p = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!p && cap != 0)
        exit(1);
    return p;
}

/* Grow an array of seq_t to at least the required capacity. */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;
    size_t new_cap = (*cap == 0) ? 256 : *cap;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *new_arr = alloc_seq(new_cap);
    for (size_t i = 0; i < *cap; ++i)
        new_arr[i] = (*arr)[i];
    free(*arr);
    *arr = new_arr;
    *cap = new_cap;
}

/* Static storage for all primes found so far. */
static seq_t *primes      = NULL;
static size_t primes_cap  = 0;
static size_t primes_size = 0;

static void push_prime(seq_t v)
{
    if (primes_size == primes_cap)
        grow_seq_array(&primes, &primes_cap, primes_size + 1);
    primes[primes_size++] = v;
}

/* Static storage for undulating primes. */
static seq_t *undul      = NULL;
static size_t undul_cap  = 0;
static size_t undul_size = 0;

static void push_undul(seq_t v)
{
    if (undul_size == undul_cap)
        grow_seq_array(&undul, &undul_cap, undul_size + 1);
    undul[undul_size++] = v;
}

/* Generate primes and collect undulating ones until we have at least n of them. */
static void generate_until(idx_t n)
{
    if (undul_size >= (size_t)n)
        return;

    /* Initialisation: ensure 2 is inserted first. */
    if (primes_size == 0) {
        push_prime((seq_t)2);
        if (is_undul((seq_t)2))
            push_undul((seq_t)2);
    }

    seq_t cand = (primes_size == 1) ? (seq_t)3 : (seq_t)1; /* placeholder, will be set below */

    /* Start from the smallest missing odd integer after the last inserted prime. */
    if (primes_size == 0 || primes_size == 1)
        cand = (seq_t)3;
    else
        cand = (seq_t)primes[primes_size - 1] + (seq_t)2;

    while (undul_size < (size_t)n) {
        /* Test cand for primality using stored primes. */
        bool is_p = true;
        for (size_t i = 0; i < primes_size; ++i) {
            seq_t p = primes[i];
            /* Stop if p > sqrt(cand).  We test p * p > cand to avoid explicit sqrt. */
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_p = false;
                break;
            }
        }

        if (is_p) {
            push_prime(cand);
            if (is_undul(cand))
                push_undul(cand);
        }

        cand += (seq_t)2; /* next odd candidate */
    }
}

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1. */
    generate_until(n);
    return undul[(size_t)n - 1];
}
