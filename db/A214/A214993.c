/*
 * A214993  Power floor sequence of (golden ratio)^5.
 * https://oeis.org/A214993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 10*a(n-2) - a(n-3) a(0) = 11, a(1) = 121,
 *            a(2) = 1341
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th term of OEIS A214993 (power floor sequence of (golden ratio)^5). */
seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)11;
    if (n == 1) return (seq_t)121;
    if (n == 2) return (seq_t)1341;

    /* Use a sliding window of the last three computed values. */
    seq_t a0 = 11;   /* a(n-3) */
    seq_t a1 = 121;  /* a(n-2) */
    seq_t a2 = 1341; /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = 12 * a2 - 10 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
