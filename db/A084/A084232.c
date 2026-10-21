/*
 * A084232  RMS values associated with A084231.
 * https://oeis.org/A084232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=195, a(n+2)=194*a(n+1)-a(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)195;

    seq_t prev = (seq_t)1;   /* a(0) */
    seq_t curr = (seq_t)195; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = ((seq_t)194 * curr) - prev; /* recurrence */
        prev = curr;
        curr = next;
    }

    return curr;
}
