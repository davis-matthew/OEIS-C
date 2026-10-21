/*
 * A074630  Numbers k such that sigma(k) == 4 mod 6.
 * https://oeis.org/A074630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 4304, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that sigma(k) == 4 mod 6 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* State of the on‑demand sieve. */
static size_t max_checked = 0;           /* largest k already processed   */
static uint8_t *sigma_arr = NULL;        /* σ(k) mod 6 for k ≤ max_checked */
static size_t sigma_cap  = 0;            /* allocated size of sigma_arr   */

/* The list of sequence terms seen so far. */
static seq_t *results = NULL;            /* stored sequence values        */
static size_t res_len  = 0;              /* number of terms stored        */
static size_t res_cap  = 0;              /* capacity of the results array */

/* Append a new term to the output array. */
static void append_term(seq_t val)
{
    if (res_len == res_cap) {
        size_t new_cap = res_cap ? res_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        results = tmp;
        res_cap = new_cap;
    }
    results[res_len++] = val;
}

/* Extend the sieve so that numbers up to new_limit have σ(k) mod 6
   computed and all qualifying terms found. */
static void extend_to(size_t new_limit)
{
    if (new_limit <= max_checked) return;

    /* Grow the σ-array if necessary, zeroing any new slots. */
    if (new_limit > sigma_cap) {
        uint8_t *tmp = (uint8_t *)realloc(sigma_arr, (new_limit + 1) * sizeof(uint8_t));
        if (!tmp) abort();
        /* Zero the new region */
        memset(tmp + sigma_cap + 1, 0, (new_limit - sigma_cap) * sizeof(uint8_t));
        sigma_arr = tmp;
        sigma_cap = new_limit;
    }

    /* Incrementally add contributions of all divisors to the new range. */
    for (size_t i = 1; i <= new_limit; ++i) {
        uint8_t add = (uint8_t)(i % 6);
        size_t start = ((max_checked + 1 + i - 1) / i) * i;
        if (start == 0) start = i;                 /* safety for i == 1 & max_checked == 0 */
        for (size_t j = start; j <= new_limit; j += i) {
            sigma_arr[j] += add;
            if (sigma_arr[j] >= 6) sigma_arr[j] -= 6;   /* keep modulo 6 */
        }
    }

    /* Record the new qualifying numbers. */
    for (size_t j = max_checked + 1; j <= new_limit; ++j) {
        if (sigma_arr[j] == 4)
            append_term((seq_t)j);
    }

    max_checked = new_limit;
}

/* Public API: return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    /* Grow the precomputed list until we have at least n terms. */
    while (res_len < (size_t)n) {
        /* Rough heuristic: double the size each time, but never
           demand more than a modest stretch. */
        size_t new_limit = max_checked ? max_checked * 2 : 128;
        if (new_limit < (res_len * 10 + 128))
            new_limit = res_len * 10 + 128;
        extend_to(new_limit);
    }
    return results[n - 1];
}
