/*
 * A123290  Number of distinct binomial(n,2)-tuples of zeros and ones that are
 *          obtained as the collection of all 2 X 2 minor determinants of a 2
 *          X n matrix over GF(2).
 * https://oeis.org/A123290
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(2n-1) - 2^n - 2^(n-1) + 4)/3 * equivalently a(n) = 1
 *            + (2^n - 1)*(2^(n-1) - 1)/3 * (works for every n ≥ 2) * One
 *            factor is always divisible by 3, so we divide it first * to keep
 *            intermediate values within the bounds of seq_t * for the 64‑bit
 *            build
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 2^exp as seq_t by repeated doubling.
 * This keeps the intermediate type seq_t so it works for
 * arbitrary‑precision builds, and in the 64‑bit build it stays
 * in 64‑bit arithmetic (overflow is acceptable for those n
 * whose result does not fit in uint64_t). */
static seq_t pow2_u(idx_t exp)
{
    seq_t r = (seq_t)1;              /* start with 1 */
    seq_t two = (seq_t)2;            /* literal 2 converts to seq_t */
    for (idx_t i = 0; i < exp; ++i)
        r = r * two;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t two_n      = pow2_u(n);       /* 2^n   */
    seq_t two_n_minus1 = pow2_u(n-1);   /* 2^(n-1) */

    if ((n & 1) == 0)                 /* n even: (2^n-1)/3 first   */
    {
        seq_t div = (two_n - (seq_t)1) / (seq_t)3;   /* divide factor */
        seq_t oth = two_n_minus1 - (seq_t)1;
        return (seq_t)1 + div * oth;
    }
    else                               /* n odd:  (2^(n-1)-1)/3 first */
    {
        seq_t div = (two_n_minus1 - (seq_t)1) / (seq_t)3;
        seq_t oth = two_n - (seq_t)1;
        return (seq_t)1 + div * oth;
    }
}
