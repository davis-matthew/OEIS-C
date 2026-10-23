/*
 * A122068  Expansion of x*(1-3*x)*(1-x)/(1-7*x+14*x^2-7*x^3).
 * https://oeis.org/A122068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies recurrence a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3)
 *            for n >= 4, with initial values a(1)=1, a(2)=3, a(3)=10
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)10;
    seq_t a1 = (seq_t)1; // a(n-3)
    seq_t a2 = (seq_t)3; // a(n-2)
    seq_t a3 = (seq_t)10; // a(n-1)
    seq_t next;
    for (idx_t i = 4; i <= n; ++i) {
        next = 7 * a3 - 14 * a2 + 7 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
