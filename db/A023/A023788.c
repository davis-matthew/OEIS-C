/*
 * A023788  Katadromes: digits in base 6 are in strict descending order.
 * https://oeis.org/A023788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: full, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all base‑6 numbers with strictly descending digits
   (digits 0–5, each occurring at most once).  The sequence consists of
   the sorted list of these numbers, starting with 0.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* comparison function for qsort */
static int cmpfunc(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static seq_t seq[64];
static bool ready = false;

/* build the (up to) 63 terms of the sequence, once */
static void init_seq(void)
{
    int idx = 0;
    seq[idx++] = (seq_t)0;                        /* zero term    */
    /* iterate over all non‑empty digit sets except {0} (mask == 1) */
    for (idx_t mask = 2; mask < (1ULL << 6); ++mask) {
        seq_t val = 0;
        for (int d = 5; d >= 0; --d) {
            if (mask & (1ULL << d)) {
                val = val * (seq_t)6 + (seq_t)d;
            }
        }
        seq[idx++] = val;
    }
    /* after this, idx == 63 */
    mem_qsort(seq, (size_t)idx, sizeof(seq_t), cmpfunc);
    ready = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!ready) init_seq();
    if (n <= 63) return seq[(size_t)(n - 1)];
    /* terms beyond the finite list are undefined – return 0 */
    return (seq_t)0;
}
