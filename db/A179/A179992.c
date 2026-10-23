/*
 * A179992  a(n) = a(n-1) + a(n-2) + n^2 for n >= 3, a(1)=2, and a(2)=5.
 * https://oeis.org/A179992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + n^2 (a(1)=2, a(2)=5)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the base cases directly. */
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)5;

    /* Use two variables to keep the last two terms. */
    seq_t prev2 = (seq_t)2; /* a(1) */
    seq_t prev1 = (seq_t)5; /* a(2) */
    seq_t cur;

    /* Iterate from 3 up to n, computing each term. */
    for (idx_t i = 3; i <= n; ++i) {
        /* n^2 is (seq_t)i * (seq_t)i to avoid overflow in idx_t */
        seq_t square = (seq_t)i * (seq_t)i;
        cur = prev1 + prev2 + square;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1; /* this is a(n) */
}
