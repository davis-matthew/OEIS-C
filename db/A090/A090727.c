/*
 * A090727  a(n) = 16a(n-1) - a(n-2), starting with a(0) = 2 and a(1) = 16.
 * https://oeis.org/A090727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 16*a(n-1) - a(n-2), a(0)=2, a(1)=16
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases */
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)16;

    /* Iterative computation of the recurrence */
    seq_t a0 = 2;      /* a(n-2) */
    seq_t a1 = 16;     /* a(n-1) */
    idx_t i;

    for (i = 2; i <= n; ++i) {
        seq_t next = 16 * a1 - a0; /* a(n) = 16*a(n-1) - a(n-2) */
        a0 = a1; /* shift for next iteration */
        a1 = next;
    }

    return a1; /* a(n) */
}
