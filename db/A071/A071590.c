/*
 * A071590  Numbers k such that reversal(k) < k.
 * https://oeis.org/A071590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n‑th positive integer k such that reverse(k) < k
 *            (where reverse is the decimal reversal with any leading zeros
 *            discarded)
 * Range:     u8 n <= 67, u16 n <= 5400, u32 n <= 5400, u64 n <= 5400,
 *            u128 n <= 5400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Reverse a 64‑bit integer – trailing zeros are dropped automatically. */
static uint64_t reverse_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x != 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* OEIS A071590 – numbers whose decimal reverse is smaller. */
seq_t a(idx_t n)
{
    /* count holds how many qualifying numbers we have found. */
    uint64_t count = 0;
    /* candidate is the current integer to test. */
    uint64_t candidate = 1;

    while (count < n) {
        uint64_t rev = reverse_uint64(candidate);
        if (candidate > rev) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
    /* The loop always returns before reaching this point; the following
       is only to keep the compiler happy. */
    return (seq_t)0;
}
