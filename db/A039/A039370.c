/*
 * A039370  Numbers whose base-9 representation has the same nonzero number of
 *          1's and 7's.
 * https://oeis.org/A039370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 8512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: returns the n‑th positive integer whose base‑9
   representation contains the same non‑zero number of 1's and 7's. */

#include <stdbool.h>

/* true if x has equal non‑zero counts of digits 1 and 7 in base 9 */
static bool valid(unsigned long long x)
{
    int cnt1 = 0, cnt7 = 0;
    while (x > 0) {
        int d = (int)(x % 9ull);
        if (d == 1) ++cnt1;
        else if (d == 7) ++cnt7;
        x /= 9ull;
    }
    return (cnt1 == cnt7) && cnt1 > 0;
}

seq_t a(idx_t n)
{
    /* We use an unsigned 64‑bit accumulator so we can test values
       larger than seq_t when needed for intermediate calculations.   */
    unsigned long long candidate = 0;
    idx_t found = 0;

    while (found < n) {
        ++candidate;
        if (valid(candidate)) {
            ++found;
        }
    }
    return (seq_t)candidate;  /* truncated on purpose for small seq_t types */
}
