/*
 * A011710  A binary m-sequence: expansion of reciprocal of x^8+x^7+x^6+x+1.
 * https://oeis.org/A011710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-8) + a(n-7) + a(n-6) + a(n-1)) mod 2, with a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=0, a(7)=1. This is
 *            the binary m‑sequence for the reciprocal of x^8+x^7+x^6+x+1. The
 *            sequence is 255‑periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for one full period */
    static seq_t seq[255];
    static bool initialized = false;

    if (!initialized) {
        /* initial 8 terms */
        for (idx_t i = 0; i < 8; ++i)
            seq[i] = 0;
        seq[7] = 1;
        /* recurrence for the rest of the period */
        for (idx_t i = 8; i < 255; ++i) {
            seq[i] = (seq[i-8] ^ seq[i-7] ^ seq[i-6] ^ seq[i-1]) & 1;
        }
        initialized = true;
    }

    return seq[n % 255];
}
