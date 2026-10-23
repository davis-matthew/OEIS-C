/*
 * A195623  Numerators of Pythagorean approximations to 5.
 * https://oeis.org/A195623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 101*a(n-1) + 101*a(n-2) - a(n-3); Initial values:
 *            a(1)=99, a(2)=10101, a(3)=1030199
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)99;
    if (n == 2) return (seq_t)10101;
    if (n == 3) return (seq_t)1030199;

    const seq_t c101 = (seq_t)101;
    seq_t a1 = (seq_t)1030199;   /* a(n-1) */
    seq_t a2 = (seq_t)10101;     /* a(n-2) */
    seq_t a3 = (seq_t)99;        /* a(n-3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = c101 * a1 + c101 * a2 - a3;
        a3 = a2;
        a2 = a1;
        a1 = next;
    }
    return a1;
}
