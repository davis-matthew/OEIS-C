/*
 * A097326  Largest integer m such that m*n has the same decimal digit length
 *          as n.
 * https://oeis.org/A097326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor((10^A055642(n) - 1)/n)
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A055642(n) is the decimal digit count of n.   */
/* The function computes the largest integer m such that m*n has the same */
/* decimal digit length as n.  It works for any n ≥ 1 and uses only  */
/* integer arithmetic on the type seq_t. */
#include <stdint.h>

/* Count decimal digits of a non‑zero unsigned integer n */
static int dec_digits_idx(idx_t n)
{
    int d = 0;
    do
    {
        d++;
        n /= 10;
    } while (n);
    return d;
}

seq_t a(idx_t n)
{
    /* According to the problem statement n ≥ 1, but guard against zero. */
    if (n == 0)
        return 0;

    /* d = number of decimal digits of n */
    int d = dec_digits_idx(n);

    /* Compute 10^d */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < (idx_t)d; ++i)
        pow10 *= 10;

    /* (10^d - 1) */
    seq_t top = pow10 - 1;

    /* Convert n to seq_t for division */
    seq_t n_seq = (seq_t)n;

    /* Largest m such that m*n < 10^d   */
    return top / n_seq;
}
