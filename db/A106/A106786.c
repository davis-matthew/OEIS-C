/*
 * A106786  Primes whose sum of digits is 59.
 * https://oeis.org/A106786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1800, u64 n <= 1800, u128 n <= 1800,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 1740 (2026-11-03)
 * Helpers:   digit_sum_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th prime whose decimal digit sum is 59. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* enlarge array to store seq_t values */
static void add_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val) {
    if (*cap == 0) {
        *cap  = 4;
        *arr  = mem_new(seq_t, (*cap));
    }
    if (*size == *cap) {
        *cap <<= 1;
        *arr  = mem_resize(seq_t, *arr, (*cap));
    }
    (*arr)[*size] = val;
    (*size)++;
}

/* Memoised storage of the primes found so far. */
static seq_t *prime_list = NULL;
static idx_t prime_cap   = 0;
static idx_t prime_cnt   = 0;
static uint64_t last_checked = 1;

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive n – behaviour below the offset is irrelevant. */
    if (n == 0)
        return (seq_t)0;

    /* Keep searching until we have at least n values stored. */
    while (prime_cnt < n)
    {
        ++last_checked;                           /* Next integer to examine */
        if (is_prime_u64(last_checked))
        {
            if (digit_sum_u64(last_checked) == 59)
            {
                add_seq(&prime_list, &prime_cap, &prime_cnt,
                        (seq_t)last_checked);
            }
        }
    }
    return prime_list[n - 1];
}
