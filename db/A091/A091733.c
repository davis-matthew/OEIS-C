/*
 * A091733  a(n) is the least m > 1 such that m^3 = 1 (mod n).
 * https://oeis.org/A091733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the least m > 1 such that m³ ≡ 1 (mod n)  (Definition of A091733) */
#include <stdbool.h>

/* Compute the smallest m > 1 with m^3 ≡ 1 (mod n) */
seq_t a(idx_t n)
{
    /* Special case n == 1 */
    if (n == 1) return 2;

    seq_t nn = (seq_t)n;          /* modulus, cast once */
    for (seq_t m = 2; m <= nn + 1; m++) {
        /* Compute m^3 mod n using repeated modular multiplication */
        seq_t t = (m * m) % nn;   /* m² mod n */
        t = (t * m) % nn;         /* m³ mod n */
        if (t == 1) return m;
    }
    /* By theorem the result is at most n+1, so this line is never reached */
    return nn + 1;
}
