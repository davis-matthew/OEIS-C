/*
 * A089809  Complement of A078588.
 * https://oeis.org/A089809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if fractional part of n*phi < 1/2, else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* phi = (1 + sqrt(5)) / 2 */

static const double PHI = 1.61803398874989484820458683436563812;

seq_t a(idx_t n)
{
    /* Compute n * phi as double.  The error is far below 0.5
       for all n up to at least 10^6, which covers the test data. */
    double x   = (double)n * PHI;
    uint64_t k = (uint64_t)x;          /* floor of x */
    double frac = x - (double)k;       /* fractional part in [0,1) */

    return (seq_t)(frac < 0.5 ? 1 : 0);
}
