/*
 * A167385  a(n)= sum_{i=7..n+6} A000931(i).
 * https://oeis.org/A167385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-4) for n >= 4 Initial values: a(0)
 *            = 1, a(1) = 3, a(2) = 5, a(3) = 8
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For small n just return the known terms. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)8;

    /* Use a ring buffer of size five to keep the last five
       calculated terms of the recurrence. */
    seq_t v[5];
    v[0] = (seq_t)1;  /* a(0)  */
    v[1] = (seq_t)3;  /* a(1)  */
    v[2] = (seq_t)5;  /* a(2)  */
    v[3] = (seq_t)8;  /* a(3)  */
    v[4] = 0;         /* placeholder for a(4) and beyond */

    idx_t i;
    for (i = 4; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-2) - a(i-4) */
        seq_t cur = v[(i - 1) % 5] + v[(i - 2) % 5] - v[(i - 4) % 5];
        v[i % 5] = cur;
    }
    return v[n % 5];
}
