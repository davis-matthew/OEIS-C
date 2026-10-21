/*
 * A236309  Total number of distinct least common multiples of (i,j) with
 *          1<=i<j<=n.
 * https://oeis.org/A236309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct lcm(i,j) for 1 ≤ i < j ≤ n
 * Range:     u8 n <= 16, u16 n <= 315, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _popcount8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/_popcount8.h"

/* Euclidean algorithm for seq_t */
static inline seq_t _gcd_seq_t(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Static bitset reused across calls */
static uint8_t *bitset = NULL;
static size_t bitset_bytes = 0;
static size_t bitset_nmax = 0;

/* Allocate or enlarge the bitset */
static void _ensure_bitset_size(size_t n)
{
    if (n <= bitset_nmax)
        return;
    size_t needed_bits = (size_t)n * n + 1;            /* max lcm ≤ n*n */
    size_t needed_bytes = (needed_bits + 7) >> 3;
    uint8_t *newb = (uint8_t *)realloc(bitset, needed_bytes);
    if (!newb) {
        /* out of memory – abort */
        exit(1);
    }
    bitset = newb;
    memset(bitset, 0, needed_bytes);
    bitset_bytes = needed_bytes;
    bitset_nmax = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;   /* no pairs */

    _ensure_bitset_size((size_t)n);

    /* clear bitset for current n */
    memset(bitset, 0, bitset_bytes);

    /* compute lcm for each pair i<j and set bit */
    for (idx_t i = 1; i < n; ++i) {
        seq_t si = (seq_t)i;
        for (idx_t j = i + 1; j <= n; ++j) {
            seq_t sj = (seq_t)j;
            seq_t g = _gcd_seq_t(si, sj);
            seq_t lcm = (si / g) * sj; /* safe: ≤ i*j ≤ n^2 */
            uint64_t idx = (uint64_t)lcm; /* lcm fits in 64 bits (≤ n^2) */
            size_t bidx = idx >> 3;
            uint8_t mask = 1u << (idx & 7);
            bitset[bidx] |= mask;
        }
    }

    /* count set bits */
    seq_t count = 0;
    for (size_t i = 0; i < bitset_bytes; ++i) {
        count += (seq_t)_popcount8(bitset[i]);
    }
    return count;
}
