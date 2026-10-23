/*
 * A175488  Triangle read by rows: T(n,m) = the largest positive integer that,
 *          when written in binary, occurs as a substring both in binary m and
 *          in binary n.
 * https://oeis.org/A175488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, tabl, nonn
 * Formula:   a(n) = largest positive integer whose binary representation
 *            appears as a contiguous substring in both row and column numbers
 *            of the triangle described in A175488
 * Range:     u8 n <= 8706, u16 n <= 8706, u32 n <= 8706, u64 n <= 8706,
 *            u128 n <= 8706, big any n
 * Helpers:   highest_bit_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include "../utils/highest_bit_uint64.h"

#define WORDS 256               /* 256 * 64 = 16384 bits (covers 2^14 numbers) */

typedef struct {
    uint64_t words[WORDS];
} Bits;

static Bits *rowSets = NULL;          /* array of bitsets for each integer index */
static idx_t rowSets_size  = 0;       
static size_t rowSets_capacity = 0;   /* current allocated capacity */

/* Ensure that bitsets for all indices up to upTo are computed. */
static void ensure_precomputed(idx_t upTo) {
    if (upTo <= rowSets_size) return;

    /* Increase capacity if necessary */
    if (rowSets_capacity < (size_t)upTo + 1) {
        size_t new_cap = rowSets_capacity ? rowSets_capacity * 2 : 8;
        if (new_cap < (size_t)upTo + 1) new_cap = upTo + 1;
        Bits *new_ptr = (Bits *)realloc(rowSets, new_cap * sizeof(Bits));
        if (!new_ptr) exit(1);
        for (size_t i = rowSets_capacity; i < new_cap; ++i)
            memset(&new_ptr[i], 0, sizeof(Bits));
        rowSets = new_ptr;
        rowSets_capacity = new_cap;
    }

    /* Compute bitsets for new indices */
    for (idx_t x = rowSets_size + 1; x <= upTo; ++x) {
        uint64_t words[WORDS];
        memset(words, 0, sizeof(words));

        /* Build binary representation of x */
        uint64_t temp = x;
        int bitsRev[64], len = 0;
        while (temp) {
            bitsRev[len++] = temp & 1;
            temp >>= 1;
        }
        int bits[64];
        for (int i = 0; i < len; ++i)
            bits[i] = bitsRev[len - 1 - i];

        /* Enumerate all substrings that start with '1' and set corresponding bits */
        for (int i = 0; i < len; ++i) {
            if (bits[i] == 0) continue;            /* must start with 1 */
            uint64_t cur = 0;
            for (int j = i; j < len; ++j) {
                cur = (cur << 1) | bits[j];
                words[cur >> 6] |= 1ULL << (cur & 0x3F);
            }
        }
        memcpy(&rowSets[x].words, words, sizeof(words));
    }
    rowSets_size = upTo;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;            /* Undefined for n < 1, return 0 */

    /* Determine row (r) such that r(r+1)/2 >= n */
    double d = (sqrt((double)(8 * (double)n + 1.0)) - 1.0) / 2.0;
    idx_t r = (idx_t)d;
    while (r * (r + 1) / 2 < n) ++r;
    idx_t prev = (r - 1) * r / 2;
    idx_t col = n - prev;

    ensure_precomputed(r);

    const uint64_t *rowBits = rowSets[r].words;
    const uint64_t *colBits = rowSets[col].words;

    /* Find the largest common bit (i.e., integer value) */
    for (int i = WORDS - 1; i >= 0; --i) {
        uint64_t inter = rowBits[i] & colBits[i];
        if (inter) {
            int pos = highest_bit_uint64(inter);
            idx_t val = ((idx_t)i << 6) | (idx_t)pos;
            return (seq_t)val;
        }
    }
    /* Fallback (should never happen for n>=1) */
    return (seq_t)1;
}
