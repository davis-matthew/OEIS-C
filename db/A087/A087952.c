/*
 * A087952  Smallest prime == 1 (mod n) and > n^2.
 * https://oeis.org/A087952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p > n² with p ≡ 1 (mod n)
 * Range:     u8 n <= 12, u16 n <= 250, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, undefined for n=0 */

    seq_t ns = (seq_t)n;           /* n in seq_t type */
    seq_t p  = ns * ns + 1;        /* first candidate ≡ 1 (mod n), and > n² */

    /* Walk along the arithmetic progression 1 + k·n until a prime is found */
    while (!isPrimeGeneric(p))
        p += ns;

    return p;
}
