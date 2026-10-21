/*
 * A084995  Numbers which can be written as the product of two different
 *          primes and the sum of digits is also prime.
 * https://oeis.org/A084995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum_uint64, factor_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084995: numbers that are the product of two distinct primes and whose digit sum is prime.
   Computation is performed by scanning integers sequentially, testing each candidate for the
   two required properties. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Utility function prototypes from the shared library. */
#include "../utils/factor_uint64.h"
#include "../utils/is_prime.h"
#include "../utils/digit_sum_uint64.h"

/* ------------------------------------------------------------------ */

/* Helper: test whether a 64‑bit integer is a product of two *distinct* primes. */
static bool semiprime_distinct_u64(uint64_t x)
{
    if (x < 6u)            /* 2*3 is the smallest semiprime with distinct primes */
        return false;

    uint64_t primes[20];
    int exp[20];
    int cnt;
    factor_uint64(x, primes, exp, &cnt);

    if (cnt != 2)           /* need exactly two distinct prime factors */
        return false;

    /* Both exponents must be 1 (distinct primes, no repetition). */
    return (exp[0] == 1) && (exp[1] == 1);
}

/* ------------------------------------------------------------------ */
/* Dynamic storage for the generated sequence.                         */
static seq_t *seq_arr   = NULL;   /* array of values */
static size_t seq_len   = 0;      /* number of values currently stored */
static size_t seq_cap   = 0;      /* allocated capacity */
static uint64_t last_checked = 1; /* last integer examined */

/* Ensure that the sequence array has space for at least min_cap elements. */
static void ensure_seq_cap(size_t min_cap)
{
    if (seq_cap >= min_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    seq_arr = tmp;
    seq_cap = new_cap;
}

/* Append an element to the sequence array. */
static void append_seq(seq_t v)
{
    ensure_seq_cap(seq_len + 1);
    seq_arr[seq_len++] = v;
}

/* ------------------------------------------------------------------ */
/* Public sequence function as required by the framework.             */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* the API expects a non‑zero index; guard anyway */

    /* Generate terms until the requested index is reached. */
    while (seq_len < (size_t)n) {
        ++last_checked;

        if (semiprime_distinct_u64(last_checked) &&
            is_prime((idx_t)digit_sum_uint64(last_checked))) {
            append_seq((seq_t)last_checked);
        }
    }
    return seq_arr[n - 1];
}
