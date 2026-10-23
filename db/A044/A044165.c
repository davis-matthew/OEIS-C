/*
 * A044165  Numbers k such that string 3,6 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that the base‑7 representation of
 *            k contains the substring "36" but the base‑7 representation of
 *            k-1 does not
 * Range:     u8 n <= 6, u16 n <= 1485, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has36
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has36.h"

/* Return the n‑th term of the sequence.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for n == 0, but guard anyway */
        return 0;

    idx_t hits = 0;                 /* number of qualifying integers found */
    idx_t k    = 0;                 /* current integer being tested */

    while (hits < n) {
        ++k;
        if (has36(k) && !has36(k - 1))
            ++hits;                 /* found a qualifying integer */
    }
    return (seq_t)k;                /* cast to the sequence's integer type */
}
