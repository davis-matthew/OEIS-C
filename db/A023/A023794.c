/*
 * A023794  Katadromes: digits in base 13 are in strict descending order.
 * https://oeis.org/A023794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 4177, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   cmp_seq_t_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th smallest integer whose base‑13 digits are in strict
 * descending order, ordered by increasing value. */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t_simple.h"

#define TOTAL 8191 /* 2^13 - 1 numbers (including 0) */

static seq_t arr[TOTAL];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* generate all numbers with strictly descending base‑13 digits */
        arr[0] = (seq_t)0;          /* the unique representation of 0 */
        size_t pos = 1;
        /* enumerate all non‑empty subsets of {0,…,12} except {0} alone (mask==1) */
        for (idx_t mask = 1; mask < ((idx_t)1 << 13); ++mask) {
            if (mask == 1) continue; /* skip duplicate 0 */
            seq_t val = 0;
            /* process digits from most significant (12) to least (0) */
            for (int d = 12; d >= 0; --d) {
                if (mask & ((idx_t)1 << d))
                    val = val * (seq_t)13 + (seq_t)d;
            }
            arr[pos++] = val;
        }
        /* sort the values so that arr[k] = a(k+1) */
        qsort(arr, TOTAL, sizeof(seq_t), cmp_seq_t_simple);
        initialized = true;
    }

    if (n == 0) return 0;                 /* not used by OEIS, safe return */
    idx_t idx = n - 1;
    if (idx >= (idx_t)TOTAL) return 0;    /* n beyond the finite sequence */
    return arr[idx];
}
