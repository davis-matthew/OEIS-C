/*
 * A018536  Divisors of 592.
 * https://oeis.org/A018536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 592 in ascending order; a(1)=1, a(2)=2,
 *            a(3)=4, a(4)=8, a(5)=16, a(6)=37, a(7)=74, a(8)=148, a(9)=296,
 *            a(10)=592; a(n)=0 for n>10
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first five divisors are powers of two */
    if (n <= 5) {
        return (seq_t)1 << (n-1);
    }
    /* next five divisors are 37 multiplied by powers of two */
    if (n <= 10) {
        return (seq_t)37 * ((seq_t)1 << (n-6));
    }
    /* sequence ends after 10 terms */
    return 0;
}
