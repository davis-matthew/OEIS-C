/*
 * A055158  n has distinct digits in descending order and n=a+b where a has
 *          the digits of n in another order and b has the digits of n in
 *          ascending order (perhaps with leading zeros).
 * https://oeis.org/A055158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, base, nonn
 * Formula:   a(n) = the nth ascending‑sorted integer that has * distinct
 *            digits in descending order and can be written as * n = a + b,
 *            where a is a permutation of those digits and * b is the digits
 *            sorted in ascending order (leading zeros allowed)
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   cmp_seq_t_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t_helper.h"

/* Cached sequence values.  A maximum of 1023 valid terms exist. */
static seq_t seq_cache[1024];
static size_t seq_len = 0;
static bool seq_built = false;

/* Build and cache the sequence once. */
static void build_sequence(void) {
    if (seq_built) return;

    seq_len = 0;
    /* Enumerate all non‑empty subsets of the digits 0‑9. */
    for (idx_t mask = 1; mask < ((idx_t)1 << 10); ++mask) {
        /* Build descending number. */
        uint64_t n_desc = 0;
        for (int d = 9; d >= 0; --d) {
            if (mask & ((idx_t)1 << d))
                n_desc = n_desc * 10 + (uint64_t)d;
        }
        /* Build ascending number. */
        uint64_t n_asc = 0;
        for (int d = 0; d <= 9; ++d) {
            if (mask & ((idx_t)1 << d))
                n_asc = n_asc * 10 + (uint64_t)d;
        }

        if (n_desc < n_asc) continue;          /* should not happen */
        uint64_t diff = n_desc - n_asc;
        if (diff == 0) continue;               /* digits cannot match mask */

        /* Compute digit mask of diff. */
        uint64_t diff_mask = 0;
        bool ok = true;
        uint64_t t = diff;
        while (t) {
            int d = t % 10;
            uint64_t bit = (uint64_t)1 << d;
            if (diff_mask & bit) {             /* repeated digit */
                ok = false;
                break;
            }
            diff_mask |= bit;
            t /= 10;
        }
        if (!ok) continue;

        if (diff_mask == mask) {
            /* Valid term.  Store the descending number. */
            seq_cache[seq_len++] = (seq_t)n_desc;
        }
    }

    /* Sort ascending for direct indexing by n. */
    qsort(seq_cache, seq_len, sizeof(seq_t), cmp_seq_t_helper);
    seq_built = true;
}

/* Public sequence function. */
seq_t a(idx_t n) {
    build_sequence();
    if (n < seq_len)
        return seq_cache[(size_t)n];
    return (seq_t)0;   /* out of range */
}
