/*
 * A197704  Integers divisible by their generalized weight.
 * https://oeis.org/A197704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer divisible by its generalized binary
 *            weight (weight assigns 1 -> 3, 0 -> 4). The function generates
 *            the sequence on demand using a simple sieve
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   weight_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/weight_bin.h"

/* Dynamic array of found terms. */
static seq_t *terms  = NULL;
static size_t capacity = 0;

/* Ensure that the array can hold at least 'needed' elements. */
static void
ensure_capacity(size_t needed)
{
    if (capacity < needed) {
        size_t new_cap = capacity ? (capacity << 1) : 8;
        while (new_cap < needed)
            new_cap <<= 1;
        /* reinterpret_cast to seq_t* for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (tmp) {
            terms = tmp;
            capacity = new_cap;
        }
    }
}

/* Public entry point requested by the framework. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; unused */
    static seq_t next_candidate = 1;
    static idx_t count = 0;

    while (count < n) {
        seq_t w = weight_bin(next_candidate);
        if (next_candidate % w == 0) {
            ensure_capacity(count + 1);
            terms[count++] = next_candidate;
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
