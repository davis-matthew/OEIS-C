/*
 * A116918  a(1)=a(2)=1. a(n) = A020639(a(n-1)) + A006530(a(n-2)).
 * https://oeis.org/A116918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=a(2)=1; a(3)=2; a(4)=3; a(5)=5; for n≥5, a(11k+5)=5,
 *            a(11k+6)=8, a(11k+7)=7, a(11k+8)=9, a(11k+9)=10, a(11k+10)=5,
 *            a(11k+11)=10, a(11k+12)=7, a(11k+13)=12, a(11k+14)=9,
 *            a(11k+15)=6
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values */
    if (n == 1 || n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)3;
    if (n == 5) return (seq_t)5;

    /* Periodic part for n >= 6 (indices 11k+6 to 11k+15) */
    const seq_t period[11] = {5, 8, 7, 9, 10, 5, 10, 7, 12, 9, 6};

    /* Compute offset within the period */
    idx_t rem = (n - 5) % 11;
    return period[rem];
}
