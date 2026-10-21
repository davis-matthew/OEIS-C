/*
 * A212669  a(n) = (2/15) * (32*n^5 + 80*n^4 + 40*n^3 - 20*n^2 + 3*n).
 * https://oeis.org/A212669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (2/15) * (32*n^5 + 80*n^4 + 40*n^3 - 20*n^2 + 3*n)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 62, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Use 128‑bit arithmetic when seq_t is an integer type
       that fits in 8 bytes.  This protects against overflow
       for n up to at least 10 000. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 M   = (unsigned __int128)n;
        unsigned __int128 n2  = M * M;
        unsigned __int128 n3  = n2 * M;
        unsigned __int128 n4  = n3 * M;
        unsigned __int128 n5  = n4 * M;

        unsigned __int128 poly =
            (unsigned __int128)32 * n5 +
            (unsigned __int128)80 * n4 +
            (unsigned __int128)40 * n3 -
            (unsigned __int128)20 * n2 +
            (unsigned __int128)3  * M;

        result = (seq_t)((poly / 15) * 2);
    } else {
        /* For arbitrary‑precision seq_t the compiler will
           use the overloaded operators provided by the library. */
        seq_t N = (seq_t)n;
        seq_t poly =
            (seq_t)32 * N * N * N * N * N +
            (seq_t)80 * N * N * N * N +
            (seq_t)40 * N * N * N -
            (seq_t)20 * N * N +
            (seq_t)3  * N;

        result = (poly / 15) * 2;
    }

    return result;
}
