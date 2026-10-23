/*
 * A123102  a(0)=1, a(1)=0, a(2)=1, a(n) = a(n-1) + a(n-2) + 3*a(n-3).
 * https://oeis.org/A123102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-3) Initial values: a(0)=1,
 *            a(1)=0, a(2)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the three base cases directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;

    /* v0 = a(i-3), v1 = a(i-2), v2 = a(i-1) at each step. */
    seq_t v0 = (seq_t)1;  /* a(0) */
    seq_t v1 = (seq_t)0;  /* a(1) */
    seq_t v2 = (seq_t)1;  /* a(2) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = v2 + v1 + 3*v0   (recurrence) */
        next = v2 + v1 + (v0 * 3);
        v0 = v1;
        v1 = v2;
        v2 = next;
    }
    return v2;
}
