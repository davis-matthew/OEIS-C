/*
 * A166396  a(n) = the number of distinct positive decimal values k of
 *          substrings in the binary representation of n where k+1 is also the
 *          value of at least one substring in the binary representation of n.
 * https://oeis.org/A166396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct positive binary substrings k of n such
 *            that k+1 also appears as a binary substring of n. The sequence
 *            is defined for n ≥ 1. For n = 2^m‑1 the value is 0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* comparison routine for qsort on 64‑bit unsigned integers */
static int cmp_uint64(const void *p, const void *q)
{
    const uint64_t *a = (const uint64_t *)p;
    const uint64_t *b = (const uint64_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Compute the sequence term a(n).  idx_t is an unsigned 64‑bit integer
   and seq_t is an unsigned integer type of sufficient size for the
   result. */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined; return 0 for safety */
        return 0;

    /* length of the binary representation of n */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        len++;
        tmp >>= 1;
    }

    /* store bits: bits[0] = most significant bit */
    unsigned char bits[64];
    for (idx_t i = 0; i < len; ++i) {
        bits[i] = (unsigned char)((n >> (len - 1 - i)) & 1u);
    }

    /* array for all non‑zero substring values; maximum possible size */
    uint64_t values[2080];
    size_t vcount = 0;

    /* enumerate all substrings and record their decimal values (>0) */
    for (idx_t i = 0; i < len; ++i) {
        uint64_t val = 0;
        for (idx_t j = i; j < len; ++j) {
            val = (val << 1) | bits[j];
            if (val != 0) {
                values[vcount++] = val;
            }
        }
    }

    /* sort to enable deduplication and adjacency checks */
    qsort(values, vcount, sizeof(uint64_t), cmp_uint64);

    /* deduplicate in‑place */
    size_t unique = 0;
    for (size_t i = 0; i < vcount; ++i) {
        if (i == 0 || values[i] != values[i - 1]) {
            values[unique++] = values[i];
        }
    }

    /* count k with k+1 also present */
    seq_t result = 0;
    for (size_t i = 0; i + 1 < unique; ++i) {
        if (values[i + 1] == values[i] + 1)
            result++;
    }

    return result;
}
