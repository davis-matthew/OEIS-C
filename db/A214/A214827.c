/*
 * A214827  a(n) = a(n-1) + a(n-2) + a(n-3), with a(0) = 1, a(1) = a(2) = 5.
 * https://oeis.org/A214827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) initial values: a(0) = 1, a(1) =
 *            a(2) = 5
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)5;

    /* Iteratively compute the recurrence */
    seq_t a0 = (seq_t)1; /* a(n-3) */
    seq_t a1 = (seq_t)5; /* a(n-2) */
    seq_t a2 = (seq_t)5; /* a(n-1) */
    seq_t a3;

    for (idx_t i = 3; i <= n; ++i)
    {
        a3 = a0 + a1 + a2;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
