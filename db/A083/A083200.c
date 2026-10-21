/*
 * A083200  Polynexus numbers of order 7.
 * https://oeis.org/A083200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^7-(n-1)^7)-(n^3-(n-1)^3))/120
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 65, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses 128‑bit arithmetic to avoid overflow for intermediate
   values while keeping the final result accurate for any type seq_t that can
   represent the returned value. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n and n-1 as unsigned 128‑bit integers for safe multiplication. */
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 n1   = nn - 1;

    /* Compute n^7 and (n-1)^7. */
    unsigned __int128 pow_n7   = 1;
    unsigned __int128 pow_n1_7 = 1;
    for (int i = 0; i < 7; ++i) {
        pow_n7   *= nn;
        pow_n1_7 *= n1;
    }

    /* Compute n^3 and (n-1)^3. */
    unsigned __int128 pow_n3   = 1;
    unsigned __int128 pow_n1_3 = 1;
    for (int i = 0; i < 3; ++i) {
        pow_n3   *= nn;
        pow_n1_3 *= n1;
    }

    /* Numerator: (n^7-(n-1)^7) - (n^3-(n-1)^3). */
    unsigned __int128 numerator = (pow_n7   - pow_n1_7)
                                - (pow_n3   - pow_n1_3);

    /* Divide by 120 (constant factor). The division yields an exact integer. */
    unsigned __int128 result = numerator / 120;

    return (seq_t)result;
}
