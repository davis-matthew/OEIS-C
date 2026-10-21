/*
 * A087170  Expansion of (1 + 4*x)/(1 + 7*x + 16*x^2).
 * https://oeis.org/A087170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -7*a(n-1) - 16*a(n-2), a(0)=1, a(1)=-3
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return -3;

    seq_t a0 = 1;   /* a(0) */
    seq_t a1 = -3;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (-7) * a1 - (16) * a0; /* a(i) = -7*a(i-1) - 16*a(i-2) */
        a0 = a1;
        a1 = next;
    }
    return a1;
}
