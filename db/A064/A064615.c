/*
 * A064615  Numbers of the form m * 6^k for k >= 0 and m > 0 with gcd(m, 6) =
 *          1.
 * https://oeis.org/A064615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9482 (2026-11-01)
 * Helpers:   ctz_seq_to_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th positive integer whose
   exponent of 2 equals the exponent of 3
   (equivalently, numbers of the form m·6^k with gcd(m,6)=1). */

#include <stdlib.h>
#include "../utils/ctz_seq_to_u32.h"

/* Count how many times 3 divides x. */
static inline unsigned int exp3_of_seq(seq_t x)
{
    unsigned int c = 0;
    while ((x % (seq_t)3) == (seq_t)0) {
        x /= (seq_t)3;
        ++c;
    }
    return c;
}

/* Return the n‑th term of the sequence (n ≥ 1).  The function
   works for any seq_t (built‑in unsigned integer, or an
   arbitrary‑precision big‑integer class that supports %, /, and ++). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */

    idx_t found = 0;
    seq_t value  = 0;

    while (found < n) {
        ++value;                          /* examine next integer */

        if (ctz_seq_to_u32(value) == exp3_of_seq(value))
            ++found;
    }

    return value;                         /* value is the desired term */
}
