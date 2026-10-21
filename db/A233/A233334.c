/*
 * A233334  a(1) = 1; for n > 1, a(n) is the smallest number > a(n-1) such
 *          that a(1) + a(2) + ... + a(n) is a composite number.
 * https://oeis.org/A233334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Initial values: a(1)=1 a(2)=3 a(3)=4 a(4)=6 a(5)=7 a(6)=9
 *            a(7)=10 a(8)=11 Recurrence for n>7: a(n) = 2*a(n-1) - a(n-2) The
 *            recurrence simplifies to the linear formula a(n) = n + 3 for all
 *            n >= 9 (The sequence thereafter is simply the consecutive
 *            integers starting at 12 for n = 9, 13 for n = 10, a...
 * Range:     u8 n <= 252, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial segment explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)6;
    if (n == 5) return (seq_t)7;
    if (n == 6) return (seq_t)9;
    if (n == 7) return (seq_t)10;
    if (n == 8) return (seq_t)11;

    /* For n >= 9, use the closed form a(n) = n + 3 */
    return (seq_t)n + (seq_t)3;
}
