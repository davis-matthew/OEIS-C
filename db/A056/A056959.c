/*
 * A056959  In repeated iterations of function m -> m/2 if m even, m -> 3m+1
 *          if m odd, a(n) is maximum value achieved if starting from n.
 * https://oeis.org/A056959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest value reached by the Collatz iteration
 *            starting at n. The iteration follows the rules m → m/2 if m is
 *            even, m → 3m+1 if m is odd, and we run until the cycle 1,4,2,1
 *            is entered. The first term of the sequence (offset 1) is a(1)=4,
 *            a(2)=4, a(3)=16,
 * Range:     u8 n <= 26, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The function is implemented recursively for simplicity and
   clarity.  The iteration is purely arithmetic, using 128-bit
   intermediates to avoid overflow for 64‑bit builds.  No external
   dependencies are required. */

#include <stdint.h>

/* Helper to compute next Collatz step using 128‑bit intermediate to
   avoid overflow for 64‑bit seq_t.  The resulting value is cast back
   to seq_t.  For arbitrary‑precision seq_t this cast is harmless. */
static inline seq_t collatz_next(seq_t x)
{
    if (x & 1) {
        __int128 y = (__int128)x * 3 + 1;
        return (seq_t)y;
    } else {
        return x >> 1;
    }
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 0, guard anyway */

    seq_t current = (seq_t)n;
    seq_t max_val  = current;

    /* While we haven't reached the repeating cycle, keep iterating. */
    while (current != 1) {
        current = collatz_next(current);
        if (current > max_val) max_val = current;
    }

    /* For n = 1 or 2 the maximum reached in the cycle 1→4→2 is 4. */
    if (max_val < 4) max_val = 4;   /* safe for a(1) and a(2) */

    return max_val;
}
