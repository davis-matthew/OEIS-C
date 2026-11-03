/*
 * A107308  Numbers k such that (29*k - 31) is prime.
 * https://oeis.org/A107308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k such that 29*k-31 is prime
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 873 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

static seq_t *k_arr      = NULL;
static idx_t  k_cap      = 0;
static idx_t  k_size     = 0;
static idx_t  last_k     = 0;

seq_t a(idx_t n) {
    /* Ensure we have at least the first n numbers with the property. */
    while (k_size < n) {
        ++last_k;
        uint64_t candidate = 29ULL * (uint64_t)last_k - 31ULL;
        if (is_prime_u64(candidate)) {
            push_seq(&k_arr, &k_cap, &k_size, (seq_t)last_k);
        }
    }
    return k_arr[n - 1];
}
