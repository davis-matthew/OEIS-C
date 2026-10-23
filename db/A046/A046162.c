/*
 * A046162  Reduced numerators of (n-1)^2/(n^2 + n + 1).
 * https://oeis.org/A046162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = (n-1)^2 if (n-1) mod 3 ≠ 0, otherwise (n-1)^2 / 3
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n-1 to seq_t safely.  For n=1 this yields 0. */
    seq_t m = (seq_t)(n - 1);

    /* Compute the square of (n-1). */
    seq_t num = m * m;

    /* Reduce by the gcd with (n^2+n+1).  The only common divisor is 3
       when m is divisible by 3.  So we divide by 3 only in that case. */
    if ((m % 3) == 0)
        num /= 3;

    return num;
}
