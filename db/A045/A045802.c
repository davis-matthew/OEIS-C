/*
 * A045802  2-ish numbers (end in 03, 21, 29, 47).
 * https://oeis.org/A045802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) (n >= 6) Initial values: a(1) =
 *            3, a(2) = 21, a(3) = 29, a(4) = 47, a(5) = 103
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases immediately */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)21;
    if (n == 3) return (seq_t)29;
    if (n == 4) return (seq_t)47;
    if (n == 5) return (seq_t)103;

    /* Recurrence for n >= 6 */
    seq_t a1 = (seq_t)3,   /* a(n-5) */
          a2 = (seq_t)21,  /* a(n-4) */
          a3 = (seq_t)29,  /* a(n-3) */
          a4 = (seq_t)47,  /* a(n-2) */
          a5 = (seq_t)103; /* a(n-1) */
    seq_t an;

    for (idx_t i = 6; i <= n; ++i) {
        an = a5 + a2 - a1;          /* a(n-1) + a(n-4) - a(n-5) */
        /* Slide the window forward by one position */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return a5;
}
