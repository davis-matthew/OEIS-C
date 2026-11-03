/*
 * A044424  Numbers n such that string 9,2 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): nth positive integer whose decimal representation contains "92" but n-1 does not. */

#include <stddef.h>
#include <stdbool.h>

/* Test whether a number written in decimal contains the two‑digit substring 92. */
static bool contains92(seq_t x)
{
    const seq_t TEN     = (seq_t)10;
    const seq_t HUNDRED = (seq_t)100;
    const seq_t NINETY_TWO = (seq_t)92;

    /* Scan successive adjacent two‑digit blocks from least significant to most. */
    while (x >= TEN) {
        if ((x % HUNDRED) == NINETY_TWO)  /* block = 92 ? */
            return true;
        x /= TEN;
    }
    return false;
}

/* Return the n‑th term of the sequence (n≥1). */
seq_t a(idx_t n)
{
    if (n == 0)                           /* offset is 1, but behaviour for n<1 is irrelevant */
        return (seq_t)0;

    idx_t count   = 0;                     /* how many qualifying numbers have been seen   */
    idx_t candidate = 1;                   /* current integer to test                       */

    for (;;) {                             /* we know the loop will terminate                 */
        seq_t val = (seq_t)candidate;
        if (contains92(val) && !contains92(val - (seq_t)1)) {
            ++count;
            if (count == n)                /* desired term found */
                return val;
        }
        ++candidate;
    }
}
