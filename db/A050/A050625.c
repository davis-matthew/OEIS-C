/*
 * A050625  Divisible by 3^k (where k is digit length of a(n)).
 * https://oeis.org/A050625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 386, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* A050625: a(n) = the n‑th positive integer that is divisible by 3^k,
 * where k is the number of decimal digits of the integer, listed in increasing order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t remaining = n;

    /* power10 = 10^(k-1), power3 = 3^k (for current digit length k) */
    idx_t power10 = 1;   /* 10^(1-1) = 1  */
    idx_t power3  = 3;   /* 3^1 = 3      */

    while (remaining > 0) {
        idx_t low  = power10;          /* start of k‑digit range (10^(k-1)) */
        idx_t high = power10 * 10 - 1; /* end of k‑digit range (10^k−1) */

        /* first multiple of power3 that is >= low */
        idx_t start = (low + power3 - 1) / power3 * power3;

        for (idx_t m = start; m <= high; m += power3) {
            if (remaining == 1)
                return (seq_t)m;
            remaining--;
        }

        /* move to next digit length */
        power10 *= 10;
        power3  *= 3;
    }

    /* should never reach here for valid inputs */
    return 0;
}
