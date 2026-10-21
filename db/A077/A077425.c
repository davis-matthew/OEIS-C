/*
 * A077425  a(n) == 1 (mod 4) (see A016813), but not a square (i.e., not in
 *          A000290).
 * https://oeis.org/A077425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A077425
   a(n) = the n-th positive integer that is congruent to 1 (mod 4) and is not a perfect square.
   The function iterates over the arithmetic progression 1,5,9,13,... and skips any member that is a perfect square. */

/* Dependency for integer square root. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined for n=0; safeguard */

    idx_t count = 0;          /* How many valid terms have been found */
    seq_t  candidate = 1;     /* Start at 1 (the first number ≡1 mod 4) */

    /* Loop until the n-th valid term is reached */
    while (1) {
        candidate += 4;        /* Next number ≡1 mod 4 */

        seq_t r = floor_sqrt(candidate); /* Integer square root */
        if (r * r != candidate) {        /* Candidate is NOT a perfect square */
            ++count;
            if (count == n)
                return candidate;       /* Found the n-th term */
        }
    }
}
