/*
 * A008400  Crystal ball sequence for E_6 lattice.
 * https://oeis.org/A008400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 31, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A008400 (Crystal ball sequence for E_6 lattice)
 *  a(n) = 1 + (3/20)*n*(n+1)*(26*n^4 + 52*n^3 + 73*n^2 + 47*n + 42)
 *  a(0) = 1, a(1) = 73, ... */
 
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t first, as required */
    seq_t tn = n;

    /* Polynomial part: 26*n^4 + 52*n^3 + 73*n^2 + 47*n + 42 */
    seq_t n2 = tn * tn;           /* n^2 */
    seq_t n3 = n2 * tn;           /* n^3 */
    seq_t n4 = n3 * tn;           /* n^4 */
    seq_t poly = 26*n4 + 52*n3 + 73*n2 + 47*tn + 42;

    /* Multiply (3 * tn * (tn+1) * poly) / 20.
     * We remove the factor 20 from the product step‑by‑step
     * using gcd to keep the intermediate values small. */
    seq_t denom = 20;
    seq_t f0 = tn;           /* n          */
    seq_t f1 = tn + 1;       /* n + 1      */
    seq_t f2 = poly;         /* polynomial */
    seq_t f3 = 3;            /* factor 3   */
    seq_t g;

    g = gcd(f0, denom); f0 /= g; denom /= g;
    g = gcd(f1, denom); f1 /= g; denom /= g;
    g = gcd(f2, denom); f2 /= g; denom /= g;
    g = gcd(f3, denom); f3 /= g; denom /= g;
    /* denom should now be 1 */

    /* Compute the reduced product.  The order of multiplication
     * is chosen so that intermediate results stay <= final result. */
    seq_t prod = f0 * f1;
    prod *= f2;
    prod *= f3;

    /* Final value: add 1 */
    return prod + 1;
}
