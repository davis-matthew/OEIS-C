/*
 * A096607  a(n) = A096605(2*n).
 * https://oeis.org/A096607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2*n/3) – 1 when n is divisible by 3 and (n/3) is
 *            NOT a power of two; otherwise a(n) = floor(2*n/3). With the
 *            convention a(1) = 1 (offset 1)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                         /* a(1) = 1 – 1 is not produced by the general formula */

    /* floor(2*n/3) in integer arithmetic */
    seq_t val = (seq_t)n * 2 / 3;

    /* Subtract 1 only when n is a multiple of 3 and the quotient n/3 is not a power of two */
    if ((n % 3) == 0) {
        idx_t q = n / 3;
        /* q is a power of two iff (q & (q - 1)) == 0  */
        if (q & (q - 1))        /* q not a power of two */
            val -= 1;
    }

    return val;
}
