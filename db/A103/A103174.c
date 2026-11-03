/*
 * A103174  Numbers k with increasing digits such that the digits of k appear
 *          among the digits of the k-th prime number.
 * https://oeis.org/A103174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, full, fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: full, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_trial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A103174
   Numbers k with strictly increasing decimal digits such that each
   digit of k occurs among the digits of the k-th prime.
   The function returns the n‑th such number (offset 1). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_trial.h"
#include "../utils/mem.h"

/*--- dynamic array helpers for primes --------------------------------------*/
static void ensure_primes_cap(size_t *cap, uint64_t **arr, size_t min_cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t get_prime(uint64_t idx) {
    static uint64_t *prime_vec = NULL;
    static size_t prime_cap = 0;
    static size_t prime_sz  = 0;
    static uint64_t candidate = 3;      /* next odd to test */

    if (!prime_vec) { /* first call: initialise */
        ensure_primes_cap(&prime_cap, &prime_vec, 16);
        prime_vec[0] = 2;
        prime_sz = 1;
    }

    while (prime_sz < idx) {
        /* find next prime */
        while (!is_prime_trial(candidate, prime_vec, prime_sz))
            candidate += 2;

        /* add found prime */
        if (prime_sz == prime_cap)
            ensure_primes_cap(&prime_cap, &prime_vec, prime_cap << 1);

        prime_vec[prime_sz++] = candidate;
        candidate += 2;
    }
    return (seq_t)prime_vec[idx - 1];
}

static inline uint16_t digit_mask(seq_t x)
{
    uint16_t m = 0;
    while (x != 0)
    {
        uint8_t d = (uint8_t)(x % 10);
        m |= 1u << d;
        x /= 10;
    }
    return m;
}

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

seq_t a(idx_t n)
{
    /* static cache of the sequence */
    static seq_t *seq = NULL;      /* array of found k's */
    static size_t cap = 0;         /* allocated capacity (size_t as required by push_seq) */
    static size_t size = 0;        /* number of elements stored */
    static uint64_t next_k = 1;    /* next candidate k to test, fits into uint64_t because
                                     get_prime takes a uint64_t index */

    /* ignore undefined index 0 */
    if (n == 0) return 0;

    /* extend cache until it contains the n‑th term */
    while (size < n)
    {
        /* check that k has strictly increasing decimal digits */
        bool increasing = true;
        seq_t temp = (seq_t)next_k;
        int prev_digit = 10;              // larger than any decimal digit
        while (temp > 0)
        {
            int digit = (int)(temp % 10);
            if (digit >= prev_digit)
            {
                increasing = false;
                break;
            }
            prev_digit = digit;
            temp /= 10;
        }

        if (increasing)
        {
            seq_t prime_k = get_prime(next_k);
            uint16_t maskPrime = digit_mask(prime_k);
            uint16_t maskK     = digit_mask((seq_t)next_k);
            /* all digits of k must appear in the prime */
            if ( (maskK & ~maskPrime) == 0 )
            {
                push_seq(&seq, &cap, &size, (seq_t)next_k);
            }
        }
        ++next_k;
    }
    return seq[n - 1];             /* 0‑based indexing in the array */
}
