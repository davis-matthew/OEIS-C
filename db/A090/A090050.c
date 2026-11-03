/*
 * A090050  Numbers having equal length of longest contiguous block of zeros
 *          and ones in binary expansion.
 * https://oeis.org/A090050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose binary representation has
 *            equal longest contiguous runs of 0s and 1s. This implementation
 *            generates the terms by brute force
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9537 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* checks whether x has equal longest contiguous runs of 0s and 1s in binary */
static bool has_equal_run_len(uint64_t x)
{
    if (x == 0) return false;   /* 0 is not considered */

    int bl = 64 - __builtin_clzll(x);          /* number of bits in x */
    int curBit = (x >> (bl - 1)) & 1;          /* leading bit */
    int curRun = 1;
    int max0 = 0;
    int max1 = 0;

    for (int i = bl - 2; i >= 0; --i) {
        int bit = (x >> i) & 1;
        if (bit == curBit) {
            curRun++;
        } else {
            if (curBit == 0) {
                if (curRun > max0) max0 = curRun;
            } else {
                if (curRun > max1) max1 = curRun;
            }
            curBit = bit;
            curRun = 1;
        }
    }

    /* update the final run */
    if (curBit == 0) {
        if (curRun > max0) max0 = curRun;
    } else {
        if (curRun > max1) max1 = curRun;
    }

    return max0 == max1;
}

/* sequence accessor */
seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;
    static size_t sz    = 0;
    static size_t cap   = 0;
    static uint64_t next = 1;    /* next integer to test */

    if (n == 0) return (seq_t)0; /* undefined for n=0 */

    /* generate terms up to n */
    while (sz < (size_t)n) {
        if (has_equal_run_len(next)) {
            if (sz == cap) {
                size_t new_cap = cap == 0 ? 16 : cap << 1;
                seq_t *tmp = mem_resize(seq_t, seq, new_cap);
                if (!tmp) exit(EXIT_FAILURE);
                seq   = tmp;
                cap   = new_cap;
            }
            seq[sz++] = (seq_t)next;
        }
        ++next;
    }

    return seq[n - 1];
}
