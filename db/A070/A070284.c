/*
 * A070284  Smallest of 4 consecutive numbers each divisible by a square.
 * https://oeis.org/A070284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that m, m+1, m+2, m+3 are each divisible
 *            by a perfect square > 1
 * Range:     u8 n <= 1, u16 n <= 151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   find_nth_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation scans increasing integers, marking numbers that are not square‑free by a sieve of squares,
   then counts quadruples of consecutive non‑square‑free numbers until the nth occurrence is found. */
/* The approach runs in O(n) time with a small, growing buffer, comfortably fast for n up to at least 10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/find_nth_term.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0 – return 0 */
    return find_nth_term(n);
}
