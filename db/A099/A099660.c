/*
 * A099660  a(n) is the least prime following A002280[n] repdigits.
 * https://oeis.org/A099660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = least prime greater than the n‑th repdigit of 6
 *            (A002280[n]). For n = 0 the repdigit is taken as 1 so that a(0)
 *            = 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    seq_t rep = 0;

    /* Special case: the first repdigit is defined as 1. */
    if (n == 0) {
        rep = 1;
    } else {
        /* Build the repdigit consisting of n copies of the digit 6. */
        for (idx_t i = 0; i < n; ++i) {
            rep = rep * 10 + 6;
        }
    }

    /* Candidate just after the repdigit. */
    seq_t cand = rep + 1;

    /* Skip even numbers (except the prime 2). */
    if (cand != 2 && (cand & 1) == 0) {
        cand += 1;
    }

    /* Search for the next prime. */
    while (!isPrimeGeneric(cand)) {
        cand += 2;   /* step over even numbers */
    }

    return cand;
}
