/*
 * A084432  Expansion of 2/(1-x) + Sum_{k>=0} t^2(3-t)/(1+t)/(1-t)^2, where
 *          t=x^2^k.
 * https://oeis.org/A084432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084432
   Formula:
   a(1) = 2
   a(2*n) = a(n) + n + 1          (for n ≥ 1)
   a(2*n+1) = n + 2                (i.e., a(n) = n+1 for odd n > 1) */
#include <stdint.h>  /* idx_t is defined elsewhere */

seq_t a(idx_t n)
{
    /* Protect against an out‑of‑range call.  OEIS offsets at 1. */
    if (n == 0) return 0;

    if (n == 1) return (seq_t)2;

    /* Odd indices (n > 1): a(n) = n + 1 */
    if (n & 1) return (seq_t)n + 1;

    /* Even indices: a(n) = a(n/2) + n + 1 */
    seq_t half = a(n / 2);
    return half + (seq_t)n + 1;
}
