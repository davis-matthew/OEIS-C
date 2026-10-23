/*
 * A040235  Continued fraction for sqrt(251).
 * https://oeis.org/A040235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A040235 – continued fraction for sqrt(251)
   a(0) = 15
   For n >= 1: a(n) is the (n‑1)st element of the 14‑term periodic cycle
   [1, 5, 2, 1, 2, 2, 15, 2, 2, 1, 2, 5, 1, 30].
   The cycle repeats indefinitely. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction for sqrt(251) */
    static const seq_t cycle[14] = {
        (seq_t)1, (seq_t)5, (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)2,
        (seq_t)15, (seq_t)2, (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)5,
        (seq_t)1, (seq_t)30
    };

    if (n == (idx_t)0)
        return (seq_t)15;
    /* (n-1) mod 14 yields the index into the cycle for any n >= 1 */
    idx_t idx = (n - (idx_t)1) % (idx_t)14;
    return cycle[idx];
}
