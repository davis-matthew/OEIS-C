/*
 * A107909  Numbers having no consecutive zeros or no consecutive ones in
 *          binary representation.
 * https://oeis.org/A107909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th non‑negative integer whose binary
 *            representation contains no consecutive 0s or contains no
 *            consecutive 1s (i.e. at least one of those patterns is absent).
 *            This function generates the terms by enumerating integers in
 *            order and testing the required property
 * Range:     u8 n <= 133, u16 n <= 6746, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether a 64‑bit value has a run of two consecutive ones. */
static inline bool has_consecutive_ones(uint64_t x)
{
    return (x & (x >> 1)) != 0;
}

/* Length of the binary representation of x (≥1 for x>0). */
static inline unsigned bitlen_uint64(uint64_t x)
{
    unsigned len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* Test whether a 64‑bit value has a run of two consecutive zeros within its
   finite binary representation. */
static inline bool has_consecutive_zeros(uint64_t x)
{
    /* For x==0 the length is taken as 1 (single zero). */
    unsigned len = bitlen_uint64(x);
    if (len == 0) len = 1;          /* x == 0 */
    uint64_t mask = ((uint64_t)1 << len) - 1;
    uint64_t zeros = (~x) & mask;
    return (zeros & (zeros >> 1)) != 0;
}

/* Return true if x satisfies the required property. */
static inline bool valid_uint64(uint64_t x)
{
    /* Valid if NOT (consecutive zeros AND consecutive ones). */
    return !(has_consecutive_zeros(x) && has_consecutive_ones(x));
}

/* Static array to hold the sequence values already computed. */
static seq_t *seq_array = NULL;
static size_t seq_size = 0;
static size_t seq_cap  = 0;

/* Generate (or return from cache) the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Expand the cache if necessary. */
    if (seq_cap < (size_t)n + 1) {
        size_t new_cap = seq_cap ? seq_cap : 1;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;
        seq_array = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* If we already have the requested term, return it. */
    if ((size_t)n < seq_size)
        return seq_array[n];

    /* Start scanning from the next integer after the last known term. */
    uint64_t start = (seq_size == 0) ? 0 : (uint64_t)seq_array[seq_size - 1] + 1;

    for (uint64_t v = start; seq_size <= (size_t)n; ++v) {
        if (valid_uint64(v)) {
            seq_array[seq_size++] = (seq_t)v;
        }
    }

    return seq_array[n];
}
