/*
 * A109437  a(-1) = a(0) = 0, a(1) = 1; a(n) = 5a(n-1) - 5a(n-2) + a(n-3) +
 *          2*(-1)^(n+1), alternatively a(n) = 3a(n-1) + 3a(n-2) - a(n-3).
 * https://oeis.org/A109437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - a(n-3); Initial values: a(0) = 0,
 *            a(1) = 1, a(2) = 3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t a0 = (seq_t)0; /* a(n-3) */
    seq_t a1 = (seq_t)1; /* a(n-2) */
    seq_t a2 = (seq_t)3; /* a(n-1) */
    idx_t i;

    for (i = 3; i <= n; ++i) {
        seq_t an = a2 * (seq_t)3 + a1 * (seq_t)3 - a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
