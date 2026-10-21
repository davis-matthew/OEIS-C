/*
 * A231919  a(n) = 3^n + (4^n - 3^n) * (d(n) - 3), where d(n) = A000005(n).
 * https://oeis.org/A231919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = 3^n + (4^n - 3^n) * (d(n) - 3)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 15, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   divisor_count_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/pow_int.h"

/* Computes the n-th term of OEIS sequence A231919.
 * n is an unsigned 64‑bit index (offset 1).
 * seq_t is the arbitrary‑precision or fixed‑width integer type
 * supplied by the build.  All intermediate calculations are performed
 * within seq_t to preserve exactness. */
seq_t a(idx_t n)
{
    /* number of divisors of n */
    seq_t d = (seq_t)divisor_count_u64(n);

    /* powers of 3 and 4 */
    seq_t pow3 = pow_int((seq_t)3, n);
    seq_t pow4 = pow_int((seq_t)4, n);

    /* compute a(n) = 3^n + (4^n - 3^n) * (d - 3) */
    seq_t term = pow3 + (pow4 - pow3) * (d - 3);
    return term;
}
