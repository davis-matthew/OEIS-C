/*
 * A228603  a(1) = 9, a(2) = 44, a(n) = 4*(a(n-1) + a(n-2)) (n >=3).
 * https://oeis.org/A228603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(a(n-1)+a(n-2)), with a(1)=9, a(2)=44
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle trivial cases directly */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)44;
    /* Iterate the recurrence */
    seq_t prev = (seq_t)9;
    seq_t curr = (seq_t)44;
    idx_t i;
    for (i = 3; i <= n; ++i) {
        seq_t next = (seq_t)4 * (prev + curr);
        prev = curr;
        curr = next;
    }
    return curr;
}
