/*
 * A094803  Number of (s(0), s(1), ..., s(2n)) such that 0 < s(i) < 8 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n, s(0) = 1, s(2n) = 3.
 * https://oeis.org/A094803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 4*a(n-3) Initial values: a(1) = 1,
 *            a(2) = 3, a(3) = 9
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence defined for n ≥ 1; behaviour for n < 1 is irrelevant. */
    if (n == 0) return (seq_t)0;   /* optional placeholder */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)9;

    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)3;   /* a(2) */
    seq_t a3 = (seq_t)9;   /* a(3) */
    seq_t an  = a3;        /* will hold a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        an = (6 * a3) - (10 * a2) + (4 * a1);
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
