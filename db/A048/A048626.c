/*
 * A048626  Pisot sequence P(6,9).
 * https://oeis.org/A048626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3); Initial values: a(0) = 6, a(1) = 9, a(2)
 *            = 13; For n >= 3, a(n) = a(n-1) + a(n-3)
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 53, u64 n <= 111, u128 n <= 227,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial cases directly. */
    if (n == 0) return (seq_t)6;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)13;

    /* Iteratively compute subsequent terms using only the
       previous three values. */
    seq_t val[3];
    val[0] = (seq_t)6;   /* a(0) */
    val[1] = (seq_t)9;   /* a(1) */
    val[2] = (seq_t)13;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i-1) is in val[(i-1)%3];
           a(i-3) is in val[i%3] before overwrite.  */
        seq_t next = val[(i - 1) % 3] + val[i % 3];
        val[i % 3] = next;
    }
    return val[n % 3];
}
