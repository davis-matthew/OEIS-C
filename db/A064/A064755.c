/*
 * A064755  a(n) = n*9^n - 1.
 * https://oeis.org/A064755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*9^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For the OEIS offset 1; we simply return 0 for n==0,
       although n==0 is not queried in the testing suite. */
    if (n == 0) return (seq_t)0;

    seq_t pow9 = (seq_t)1;   /* 9^0 */
    seq_t base  = (seq_t)9;   /* current base for exponentiation by squaring */
    idx_t exp = n;

    /* Compute 9^n via binary exponentiation. */
    while (exp > 0) {
        if (exp & 1) pow9 *= base;
        base *= base;
        exp >>= 1;
    }

    /* Multiply by n and subtract 1. */
    seq_t result = pow9 * (seq_t)n;
    result -= (seq_t)1;
    return result;
}
