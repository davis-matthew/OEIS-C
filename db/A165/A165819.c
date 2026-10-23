/*
 * A165819  a(n) = the smallest positive integer that, when written in binary,
 *          contains both binary n and binary 2n-1 as substrings.
 * https://oeis.org/A165819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   max_overlap, unpack_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal superstring of the binary representation of n and of 2*n-1.
   The function returns the smallest positive integer whose binary
   representation contains both substrings. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/max_overlap.h"
#include "../utils/unpack_bits.h"

/* Return bit length of a non‑negative value (0 → 0). */
static size_t bitlen_seq_t(seq_t x) {
    size_t len = 0;
    while (x != 0) {
        x >>= 1;
        len++;
    }
    return len;
}

seq_t a(idx_t n) {
    const seq_t valA = (seq_t)n;
    const seq_t valB = (seq_t)(2 * n - 1);

    const size_t lenA = bitlen_seq_t(valA);
    const size_t lenB = bitlen_seq_t(valB);

    unsigned char bitsA[128], bitsB[128];
    unpack_bits(valA, bitsA, lenA);
    unpack_bits(valB, bitsB, lenB);

    const size_t overlapAB = max_overlap(bitsA, lenA, bitsB, lenB);
    const size_t overlapBA = max_overlap(bitsB, lenB, bitsA, lenA);

    const size_t len1 = lenA + lenB - overlapAB;
    const size_t len2 = lenA + lenB - overlapBA;
    const size_t Lmin = (len1 < len2) ? len1 : len2;

    bool found = false;
    uint64_t best = 0;

    const size_t max_pos_a = Lmin - lenA;
    const size_t max_pos_b = Lmin - lenB;

    for (size_t pa = 0; pa <= max_pos_a; ++pa) {
        for (size_t pb = 0; pb <= max_pos_b; ++pb) {
            unsigned char bits[128];
            for (size_t i = 0; i < Lmin; ++i) bits[i] = 2; /* unset */

            bool conflict = false;

            for (size_t i = 0; i < lenA && !conflict; ++i) {
                size_t idx = pa + i;
                unsigned char val = bitsA[i];
                if (bits[idx] == 2) bits[idx] = val;
                else if (bits[idx] != val) conflict = true;
            }
            if (conflict) continue;

            for (size_t i = 0; i < lenB && !conflict; ++i) {
                size_t idx = pb + i;
                unsigned char val = bitsB[i];
                if (bits[idx] == 2) bits[idx] = val;
                else if (bits[idx] != val) conflict = true;
            }
            if (conflict) continue;

            /* Ensure leading bit is 1. */
            if (bits[0] == 2) bits[0] = 1;
            else if (bits[0] != 1) continue;

            /* Unset bits are 0. */
            for (size_t i = 0; i < Lmin; ++i)
                if (bits[i] == 2) bits[i] = 0;

            uint64_t val = 0;
            for (size_t i = 0; i < Lmin; ++i)
                val = (val << 1) | bits[i];

            if (!found || val < best) {
                best = val;
                found = true;
            }
        }
    }
    return (seq_t)best;
}
