/*
 * A178778  Partial sums of walks of length n+1 on a tetrahedron A001998.
 * https://oeis.org/A178778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) - 12*a(n-3) + 21*a(n-4) - 9*a(n-5)
 *            for n>4 initial values: a(0)=1, a(1)=3, a(2)=7, a(3)=17, a(4)=42
 *            Implements the recurrence using seq_t arithmetic, works for all
 *            integer types (uint64_t, unsigned __int128, or
 *            arbitrary‑precision big integers)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)17;
    if (n == 4) return (seq_t)42;

    /* window of the last five terms */
    seq_t a0 = (seq_t)1;   /* a(n-5) */
    seq_t a1 = (seq_t)3;   /* a(n-4) */
    seq_t a2 = (seq_t)7;   /* a(n-3) */
    seq_t a3 = (seq_t)17;  /* a(n-2) */
    seq_t a4 = (seq_t)42;  /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)5 * a4
           - (seq_t)4 * a3
           - (seq_t)12 * a2
           + (seq_t)21 * a1
           - (seq_t)9 * a0;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
