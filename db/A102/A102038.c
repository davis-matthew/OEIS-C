/*
 * A102038  a(n+1) = n*a(n) + a(n-1), a(0)=1, a(1)=1.
 * https://oeis.org/A102038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0)=1; a(1)=1; a(n+1)=n*a(n)+a(n-1) for n≥1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first two values explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Iterative evaluation of the recurrence. */
    seq_t a_prev = (seq_t)1;   /* a(n-1), initially a(0) */
    seq_t a_curr = (seq_t)1;   /* a(n),   initially a(1) */

    /* Compute a(2) through a(n) with loop index i running from 1 to n-1. */
    for (idx_t i = 1; i < n; ++i) {
        seq_t next = ((seq_t)i) * a_curr + a_prev; /* a(i+1) = i*a(i)+a(i-1) */
        a_prev = a_curr;
        a_curr = next;
    }

    return a_curr;
}
