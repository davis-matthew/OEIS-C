/*
 * A144796  Expansion of x*(5 + 215*x - 1253*x^2 - 23*x^3)/((1 + 34*x +
 *          x^2)*(1 - 34*x + x^2)).
 * https://oeis.org/A144796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1154*a(n-2) - a(n-4) Initial values: a(1)=5, a(2)=215,
 *            a(3)=4517, a(4)=248087
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)215;
    if (n == 3) return (seq_t)4517;
    if (n == 4) return (seq_t)248087;

    /* a[0] = a(i-4), a[1] = a(i-3), a[2] = a(i-2), a[3] = a(i-1) */
    seq_t a[4] = {(seq_t)5, (seq_t)215, (seq_t)4517, (seq_t)248087};

    for (idx_t i = 5; i <= n; ++i) {
        seq_t new_val = (seq_t)1154 * a[2] - a[0];
        a[0] = a[1];
        a[1] = a[2];
        a[2] = a[3];
        a[3] = new_val;
    }
    return a[3];
}
