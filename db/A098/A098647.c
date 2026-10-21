/*
 * A098647  Trace sequence associated to the 4 X 4 Krawtchouk matrix and its
 *          transpose.
 * https://oeis.org/A098647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 24*a(n-1) - 64*a(n-2), a(0)=1, a(1)=12
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A098647 via its linear recurrence. */
seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    seq_t a0 = (seq_t)1;      /* a(0) */
    seq_t a1 = (seq_t)12;     /* a(1) */
    idx_t i;

    for (i = 2; i <= n; ++i) {
        seq_t next = (seq_t)(24 * a1) - (seq_t)(64 * a0);
        a0 = a1;
        a1 = next;
    }

    return a1;
}
