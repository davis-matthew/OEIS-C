/*
 * A106779  Primes with digit sum = 49.
 * https://oeis.org/A106779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2700, u64 n <= 2700, u128 n <= 2700,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 2629 (2026-11-03)
 * Helpers:   digit_sum_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates primes with digit sum 49 by iteratively testing
   natural numbers and storing those that satisfy both conditions in a static
   array. It reuses the generated list for subsequent calls. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static idx_t arr_cap = 0, arr_size = 0;
    static uint64_t cur = 1;

    while (arr_size < n) {
        cur++;
        if (is_prime_u64(cur) && digit_sum_u64(cur) == (seq_t)49) {
            add_seq(&arr, &arr_cap, &arr_size, (seq_t)cur);
        }
    }

    return arr[n - 1];
}
