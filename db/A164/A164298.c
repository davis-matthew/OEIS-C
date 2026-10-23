/*
 * A164298  a(n) = ((1+4*sqrt(2))*(2+sqrt(2))^n +
 *          (1-4*sqrt(2))*(2-sqrt(2))^n)/2.
 * https://oeis.org/A164298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2); a(0) = 1, a(1) = 10
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    seq_t a0 = (seq_t)1;  /* a(n-2)  */
    seq_t a1 = (seq_t)10; /* a(n-1)  */
    seq_t a2;

    for (idx_t i = 2; i <= n; ++i) {
        a2 = a1 * (seq_t)4 - a0 * (seq_t)2;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
