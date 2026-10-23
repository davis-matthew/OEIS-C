/*
 * A142696  Primes congruent to 50 mod 57.
 * https://oeis.org/A142696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes congruent to 50 modulo 57 */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime.h"

static seq_t *results = NULL;          /* cached terms */
static size_t results_len = 0;         /* number of cached terms   */
static size_t results_cap = 0;         /* capacity of the cache    */
static uint64_t next_candidate = 107;  /* first prime in the AP    */

/* Max value storable in seq_t (uint8_t, uint16_t, uint32_t, uint64_t, …) */
static const unsigned long long seq_t_max =
    (sizeof(seq_t) == 1) ? 255ULL :
    (sizeof(seq_t) == 2) ? 65535ULL :
    (sizeof(seq_t) == 4) ? 4294967295ULL :
    ULLONG_MAX;  /* covers 64‑bit and the 128‑bit arbitrary‑precision type */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* out‑of‑range index – value irrelevant */

    /* Generate terms until we have at least n cached */
    while (results_len < n)
    {
        /* Test primality of the current candidate (uint64_t works for all seq_t) */
        if (is_prime((idx_t)next_candidate))
        {
            /* Only store the term if it fits in the seq_t type */
            if ((unsigned long long)next_candidate <= seq_t_max)
            {
                if (results_len == results_cap)
                {
                    size_t new_cap = results_cap ? results_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();  /* allocation failure */
                    results = tmp;
                    results_cap = new_cap;
                }
                results[results_len++] = (seq_t)next_candidate;
            }
        }

        /* Advance to the next number in the arithmetic progression */
        next_candidate += 57;
    }

    return results[n - 1];
}
