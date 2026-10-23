/*
 * A046729  Expansion of 4*x/((1+x)*(1-6*x+x^2)).
 * https://oeis.org/A046729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=4, a(2)=20 a(n) = 5*a(n-1) + 5*a(n-2) - a(n-3) for
 *            n >= 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A046729 (OEIS) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)20;

    seq_t f0 = 0, f1 = 4, f2 = 20, f3;

    for (idx_t i = 3; i <= n; ++i) {
        f3 = (seq_t)5 * f2 + (seq_t)5 * f1 - f0;
        f0 = f1;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}
