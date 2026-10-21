/*
 * A228554  x-values in the solution to the Pell equation x^2 - 85*y^2 = -1.
 * https://oeis.org/A228554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 571538*a(n-1) - a(n-2) Initial values: a(1) = 378, a(2) =
 *            216041742
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)378;
    if (n == 2)
        return (seq_t)216041742;
    seq_t prev = (seq_t)378;
    seq_t curr = (seq_t)216041742;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)((seq_t)571538 * curr - prev);
        prev = curr;
        curr = next;
    }
    return curr;
}
