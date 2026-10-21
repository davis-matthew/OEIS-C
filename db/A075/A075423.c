/*
 * A075423  a(n) = rad(n) - 1, where rad(n) is the squarefree kernel of n
 *          (A007947).
 * https://oeis.org/A075423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rad(n) - 1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = (product of the distinct prime factors of n) - 1  */
/*  Uses the provided rad utility for the square‑free kernel.  */
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    /* rad(1) == 1, so for n==1 we correctly return 0. */
    seq_t r = rad(n);
    return r - 1;
}
