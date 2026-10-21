/*
 * A057693  Number of permutations on n letters that have only cycles of
 *          length 3 or less.
 * https://oeis.org/A057693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (n-1)*a(n-2) + (n-1)(n-2)*a(n-3) with initial
 *            values a(0)=1, a(1)=1, a(2)=2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle small n directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Iteratively compute up to n using only three variables. */
    seq_t a0 = (seq_t)1;          /* a(k-3)  */
    seq_t a1 = (seq_t)1;          /* a(k-2)  */
    seq_t a2 = (seq_t)2;          /* a(k-1)  */
    seq_t next;
    for (idx_t i = 3; i <= n; ++i) {
        /* next = a(k-1) + (k-1)*a(k-2) + (k-1)(k-2)*a(k-3) */
        next = a2
             + (seq_t)(i - 1) * a1
             + (seq_t)(i - 1) * (seq_t)(i - 2) * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
