/*
 * A190245  a(n) = [nu+nv]-[nu]-[nv], where u=sqrt(2), v=2u, and []=floor.
 * https://oeis.org/A190245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3*√2*n) - floor(√2*n) - floor(2*√2*n) Equivalent
 *            integer expression avoiding floating-point: a(n) =
 *            floor_sqrt(18*n²) - floor_sqrt(2*n²) - floor_sqrt(8*n²)
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 7053, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t and compute n² */
    seq_t n2 = (seq_t)n * (seq_t)n;

    /* Compute the three floor square roots */
    seq_t t1 = floor_sqrt((seq_t)18 * n2);  /* floor(√(18 n²)) = floor(3√2 n) */
    seq_t t2 = floor_sqrt((seq_t) 2 * n2);  /* floor(√(2 n²))  = floor(√2 n)   */
    seq_t t3 = floor_sqrt((seq_t) 8 * n2);  /* floor(√(8 n²))  = floor(2√2 n)  */

    /* Return the difference */
    return t1 - t2 - t3;
}
