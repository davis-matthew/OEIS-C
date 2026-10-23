/*
 * A138168  Numbers containing their length in binary representation.
 * https://oeis.org/A138168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the nth integer whose binary representation contains the binary
 * representation of its own length (number of bits).  The sequence starts
 * with a(1) = 1. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

static seq_t *seq_cache = NULL;      /* cached sequence values   */
static size_t cache_size = 0;        /* number of values stored  */
static size_t cache_cap   = 0;        /* allocated capacity       */
static idx_t  next_test   = 1;        /* next integer to test     */

/* Test whether m's binary representation contains the binary
 * representation of its own bit-length. */
static bool contains_its_length(seq_t m) {
    /* Length of m in bits */
    unsigned int len_m = bitlen_seq_t(m);      /* 1 .. 128 */
    /* Binary representation of len_m */
    unsigned int len_len = bitlen_seq_t(len_m);
    seq_t target = (seq_t)len_m;
    /* mask for the len_len-bit window */
    seq_t mask = ((seq_t)1 << len_len) - 1;

    unsigned int shift_end = len_m - len_len;
    for (unsigned int pos = 0; pos <= shift_end; ++pos) {
        seq_t window = (m >> (shift_end - pos)) & mask;
        if (window == target)
            return true;
    }
    return false;
}

/* Ensure that the dynamic cache has at least 'need' capacity. */
static void ensure_cache_capacity(size_t need) {
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Public function returning the nth sequence term. */
seq_t a(idx_t n) {
    if (n < 1)
        return 0; /* undefined for offset < 1 */

    while (cache_size < (size_t)n) {
        seq_t candidate = (seq_t)next_test;
        if (contains_its_length(candidate)) {
            ensure_cache_capacity(cache_size + 1);
            seq_cache[cache_size++] = candidate;
        }
        /* advance to the next integer to test */
        next_test++;
    }
    return seq_cache[(size_t)n - 1];
}
