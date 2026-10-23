/*
 * A144016  a(n) = the largest positive integer m such that the binary
 *          representations of all positive integers <= m are found within the
 *          binary representation of n.
 * https://oeis.org/A144016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the largest m such that binary representations of all
 *            integers 1..m occur as substrings of the binary representation
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates all substrings of n, sorts them, and
   scans to find the first missing positive integer.  The algorithm
   runs in O(L^2 log L^2) where L is the bit length of n (≤64 for
   idx_t).                                        */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Comparator for qsort on uint64_t values */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, but the function may be called with 0. */
    if (n == 0)
        return (seq_t)0;

    /* Bit length L of n (1‑based). */
    unsigned L = 0;
    idx_t t = n;
    while (t) {
        L++;
        t >>= 1;
    }
    /* Bits of n, most significant bit first. */
    uint8_t bits[64];
    for (unsigned i = 0; i < L; ++i)
        bits[i] = (n >> (L - 1 - i)) & 1U;

    /* Number of possible substrings is L*(L+1)/2. */
    size_t totalSubs = ((size_t)L * (L + 1)) / 2;
    uint64_t *vals = (uint64_t *)malloc(totalSubs * sizeof(uint64_t));
    if (!vals)
        return (seq_t)0;   /* allocation failure – unlikely in judge envs */

    /* Enumerate all substrings, recording their integer values. */
    size_t count = 0;
    for (unsigned start = 0; start < L; ++start) {
        uint64_t val = 0;
        for (unsigned end = start; end < L; ++end) {
            val = (val << 1) | bits[end];
            if (val != 0)          /* ignore the zero value */
                vals[count++] = val;
        }
    }

    /* Sort and deduplicate the collected values. */
    qsort(vals, count, sizeof(uint64_t), cmp_uint64);

    /* Scan for the first missing positive integer. */
    uint64_t m = 1;          /* candidate missing integer */
    size_t idx = 0;          /* index in sorted vals */
    while (idx < count) {
        /* Skip values less than current candidate. */
        while (idx < count && vals[idx] < m)
            ++idx;

        if (idx < count && vals[idx] == m) {
            /* Skip all duplicates equal to m. */
            while (idx < count && vals[idx] == m)
                ++idx;
            ++m;  /* check next integer */
        } else {
            /* vals[idx] > m or idx == count → missing m. */
            break;
        }
    }

    free(vals);  /* clean up dynamic array */

    /* m is the smallest missing positive integer; answer is m-1. */
    return (seq_t)(m - 1);
}
