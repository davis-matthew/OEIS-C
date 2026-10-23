/*
 * A022386  Fibonacci sequence beginning 4, 26.
 * https://oeis.org/A022386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 4, a(1) = 26
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial terms directly. */
    if (n == 0)
        return (seq_t)4;
    if (n == 1)
        return (seq_t)26;

    /* Iteratively compute the recurrence. */
    seq_t a0 = (seq_t)4;   /* a(0) */
    seq_t a1 = (seq_t)26;  /* a(1) */
    seq_t an = a1;         /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        an = a1 + a0; /* a(i) = a(i-1) + a(i-2) */
        a0 = a1;      /* shift: a(i-2) becomes a(i-1) */
        a1 = an;      /* shift: a(i-1) becomes a(i)   */
    }

    return an;
}
