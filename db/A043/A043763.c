/*
 * A043763  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 0 mod 10.
 * https://oeis.org/A043763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer whose binary representation has a number
   of runs congruent to 0 modulo 10 (OEIS A043763).  The solution
   performs a linear search, counting qualifying numbers until the
   requested index is reached.  No dynamic memory allocation is used. */

#include <stdint.h>

/* Count the number of runs (maximal blocks of equal bits) in the
   binary expansion of x.  The algorithm is independent of seq_t. */
static int binary_runs(idx_t x)
{
    int runs = 0;
    int prev = -1;              /* sentinel for “no previous bit” */
    while (x) {
        int cur = x & 1;
        if (cur != prev) {
            runs++;
            prev = cur;
        }
        x >>= 1;
    }
    return runs;
}

/* OEIS sequence accessor.  idx_t is an unsigned 64‑bit type.  seq_t
   can be an unsigned integer of any size or an arbitrary‑precision
   integer class; casting from idx_t to seq_t is valid for all
   supported builds. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for index 0 */

    idx_t val = 1;                 /* candidate numbers start at 1   */
    idx_t cnt = 0;                 /* how many qualifying numbers found */

    while (cnt < n) {
        if (binary_runs(val) % 10 == 0) {
            ++cnt;
            if (cnt == n) return (seq_t)val;
        }
        ++val;
    }
    /* Control should never reach here for n >= 1 */
    return (seq_t)0;
}
