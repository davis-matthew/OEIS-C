/*
 * A037382  Numbers k such that every base-3 digit of k is a base-6 digit of
 *          k.
 * https://oeis.org/A037382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n-th positive integer k such that every base‑3
 * digit of k appears as a digit in the base‑6 representation of k. */

#include <stdbool.h>

/* Check if a number satisfies the property */
static bool satisfies_seq_t(seq_t x)
{
    uint8_t mask3 = 0;
    seq_t t = x;
    while (t > 0) {
        mask3 |= 1U << (int)(t % 3);
        t /= 3;
    }

    uint8_t mask6 = 0;
    t = x;
    while (t > 0) {
        mask6 |= 1U << (int)(t % 6);
        t /= 6;
    }

    return (mask3 & mask6) == mask3;
}

/* Return the n-th term of the sequence (1‑based) */
seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 1;            /* loop counter as 64‑bit unsigned */
    for (;;) {
        seq_t sk = (seq_t)k;   /* cast to the sequence type */
        if (satisfies_seq_t(sk)) {
            ++count;
            if (count == n)
                return sk;
        }
        ++k;
    }
}
