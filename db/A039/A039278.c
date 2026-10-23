/*
 * A039278  Numbers whose base-4 representation has the same nonzero number of
 *          0's and 2's.
 * https://oeis.org/A039278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generating OEIS A039278: Numbers whose base‑4 representation has an
 * equal non‑zero number of 0's and 2's.  The function enumerates
 * integers in increasing order, counting those that satisfy the
 * property until the requested index is reached.  This straightforward
 * search is fast enough for n ≈ 10 000. */
#include <stdbool.h>
#include <stdint.h>

/* Test whether a nonnegative integer has an equal non‑zero number of
 * 0’s and 2’s in its base‑4 representation. */
static bool satisfies(idx_t v)
{
    unsigned zeros = 0, twos = 0;
    while (v != 0) {
        unsigned d = (unsigned)(v & 3U); /* v % 4  */
        if (d == 0U)   zeros++;
        else if (d == 2U) twos++;
        v >>= 2; /* v /= 4  */
    }
    return zeros > 0 && zeros == twos;
}

/* Return the n‑th value of the sequence (1‑based). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 0;
    while (count < n) {
        candidate++;
        if (satisfies(candidate)) {
            count++;
        }
    }
    return (seq_t)candidate;
}
