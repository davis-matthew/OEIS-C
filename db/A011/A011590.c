/*
 * A011590  Legendre symbol (n,41).
 * https://oeis.org/A011590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n, 41) (offset 0, so a(0) = 0)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Legendre symbol (n|41). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* reduce n modulo the odd prime 41 */
    uint64_t r = n % 41ULL;
    if (r == 0)         /* divisible by 41 */
        return (seq_t)0;

    /* Euler's criterion: a^((p-1)/2) mod p == 1 or p-1 (i.e. 40) */
    uint64_t res = 1;
    uint64_t base = r % 41ULL;
    int e = 20;                 /* (41-1)/2 = 20 */
    while (e) {
        if (e & 1)
            res = (res * base) % 41ULL;
        base = (base * base) % 41ULL;
        e >>= 1;
    }

    /* res is 1 or 40 (since r != 0)
       1  -> residue, return +1
       40 -> nonresidue, return -1
    */
    if (res == 1)
        return (seq_t)1;
    else
        return (seq_t)-1;
}
