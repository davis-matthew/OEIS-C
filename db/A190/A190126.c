/*
 * A190126  Numbers 1 through 10000 sorted lexicographically in binary
 *          representation.
 * https://oeis.org/A190126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _cmp_bin, _log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the permutation defined by sorting the integers 1..10000
 * by lexicographic order of their binary representations. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_cmp_bin.h"
#include "../utils/_log2_uint64.h"

/* Static workspace for the permutation. */
static idx_t _sorted[10000];
static bool _sorted_initialized = false;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* Build the permutation on first call. */
    if (!_sorted_initialized) {
        for (idx_t i = 0; i < 10000; ++i) _sorted[i] = i + 1;
        qsort(_sorted, 10000, sizeof(idx_t), _cmp_bin);
        _sorted_initialized = true;
    }

    /* The sequence is defined for 1 ≤ n ≤ 10000. */
    if (n > 10000) return (seq_t)0;
    return (seq_t)_sorted[n - 1];
}
