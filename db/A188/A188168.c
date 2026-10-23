/*
 * A188168  a(0) = a(1) = 1; a(n) = 5*a(n-1) + 5*a(n-2).
 * https://oeis.org/A188168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = a(1) = 1; a(n) = 5*a(n-1) + 5*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly */
    if (n == 0 || n == 1) return (seq_t)1;

    /* a0 holds a(n-2), a1 holds a(n-1) */
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)1; /* a(1) */

    /* Iterate from 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)5 * a1 + (seq_t)5 * a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
