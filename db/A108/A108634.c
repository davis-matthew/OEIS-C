/*
 * A108634  Semiprimes with ordered digits.
 * https://oeis.org/A108634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 601, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits_ordered, is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of semiprimes with nondecreasing decimal digits
   (OEIS A108634) */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_semiprime_uint64.h"
#include "../utils/digits_ordered.h"

static seq_t *seq_array = NULL;   /* dynamic array of found terms */
static size_t   seq_cap   = 0;
static idx_t    seq_size  = 0;
static uint64_t next_candidate = 4;   /* smallest number to test */

/* Ensure the array can hold at least n elements. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < (size_t)n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_array = tmp;
    seq_cap = new_cap;
}

/* Generate terms of the sequence until at least n terms are stored. */
static void generate_up_to(idx_t n)
{
    idx_t max_needed = n;
    while (seq_size < max_needed) {
        if (is_semiprime_uint64(next_candidate) && digits_ordered(next_candidate)) {
            ensure_capacity(seq_size + 1);
            seq_array[seq_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) abort();  /* index is 1‑based, but we guard anyway */
    generate_up_to(n);
    return seq_array[n - 1];
}
