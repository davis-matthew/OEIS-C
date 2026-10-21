/*
 * A056546  a(n) = 5*n*a(n-1) + 1 with a(0)=1.
 * https://oeis.org/A056546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n) = 5 * n * a(n-1) + 1 for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t val = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) /* iterate up to n */
    {
        val = ((seq_t)5 * (seq_t)i * val) + (seq_t)1;
    }
    return val;
}
