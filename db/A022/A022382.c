/*
 * A022382  Fibonacci sequence beginning 4, 10.
 * https://oeis.org/A022382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 4, a(1) = 10
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)10;

    /* Iteratively compute the sequence for n >= 2 */
    seq_t prev = (seq_t)4;   /* a(0) */
    seq_t curr = (seq_t)10;  /* a(1) */
    seq_t next;

    /* Since n is unsigned, the loop stops safely at n */
    for (idx_t i = 2; i <= n; ++i) {
        next = prev + curr; /* a(i) = a(i-1) + a(i-2) */
        prev = curr;
        curr = next;
    }
    return curr;
}
