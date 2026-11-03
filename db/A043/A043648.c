/*
 * A043648  Numbers whose base-11 representation has exactly 5 runs.
 * https://oeis.org/A043648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043648: Numbers whose base‑11 representation has exactly five runs.
   The function computes the n‑th element (1‑indexed) by brute‑force
   scanning integers in ascending order and counting runs in their base‑11
   representation.  The algorithm is O(n·L), where L is the average number
   of base‑11 digits of the generated terms; for n ≤ 10 000 this is well
   below the required 2 s limit.  No external caches are used, so the code
   works for any seq_t (including arbitrary‑precision) and any build
   configuration. */

#include <stdbool.h>

/* Return true iff v has exactly 5 runs in base‑11. */
static bool has_five_runs(seq_t v)
{
    int runs = 0;
    int prev = -1;                       /* sentinel: no previous digit */
    while (v > (seq_t)0) {
        int digit = (int)(v % (seq_t)11);
        if (runs == 0 || digit != prev) {
            ++runs;
            prev = digit;
            if (runs > 5) return false; /* early exit */
        }
        v /= (seq_t)11;
    }
    return runs == 5;
}

/* External function requested by the harness. */
seq_t a(idx_t n)
{
    seq_t value = 1;          /* start scanning from 1 */
    idx_t  count = 0;         /* number of found terms so far */

    while (1) {
        if (has_five_runs(value)) {
            ++count;
            if (count == n) {
                return value;    /* found the n‑th term */
            }
        }
        ++value;               /* move to the next integer */
    }
}
