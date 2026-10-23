/*
 * A132009  a(1) = 1; for n>=2, a(n) = n-th positive integer which is coprime
 *          to the largest prime divisor of n.
 * https://oeis.org/A132009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer coprime with the largest prime
 *            divisor of n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) is defined to be 1 */

    uint64_t p = gpf((uint64_t)n);        /* largest prime divisor of n */
    seq_t n_seq = (seq_t)n;
    seq_t p_seq = (seq_t)p;

    /* All positive integers not divisible by p are those not congruent
       to 0 modulo p.  They come in blocks of length p, each block
       contributing p‑1 valid numbers.  The n‑th such number is:  */
    seq_t t = (n_seq - 1) / (p_seq - 1);      /* number of full blocks before it */
    seq_t offset = (n_seq - 1) % (p_seq - 1); /* position inside the current block */
    return t * p_seq + offset + 1;           /* the desired integer */
}
