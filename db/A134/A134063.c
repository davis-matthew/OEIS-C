/*
 * A134063  a(n) = (1/2)*(3^n - 2^(n+1) + 3).
 * https://oeis.org/A134063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(3^n - 2^(n+1) + 3), a(0)=1, a(1)=1, a(2)=2 *
 *            Implemented by the recurrence * a(n) = 6*a(n-1) - 11*a(n-2) +
 *            6*a(n-3) for n >= 3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;

    /* Use the recurrence. */
    seq_t a0 = 1;    /* a(0) */
    seq_t a1 = 1;    /* a(1) */
    seq_t a2 = 2;    /* a(2) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)6 * a2 - (seq_t)11 * a1 + (seq_t)6 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
