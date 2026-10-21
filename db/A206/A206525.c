/*
 * A206525  a(n) = 113*(n-1) - a(n-1) with n>1, a(1)=51.
 * https://oeis.org/A206525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-113-91*(-1)^n+226*n)/4 (equivalently a(n)=a(n-2)+113
 *            with a(1)=51, a(2)=62)
 * Range:     u8 n <= 2, u16 n <= 327, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour below the offset doesn't matter */
        return (seq_t)0;   /* return 0 for n=0 (unused but safe) */

    seq_t temp = (seq_t)226 * n;
    if (n & 1)
        temp -= (seq_t)22;   /* numerator = 226*n - 22  (n odd) */
    else
        temp -= (seq_t)204;  /* numerator = 226*n - 204 (n even) */

    return temp / 4;
}
