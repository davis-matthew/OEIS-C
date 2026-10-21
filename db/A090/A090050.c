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
 * Helpers:   has_equal_run_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_run_len.h"

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
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
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
