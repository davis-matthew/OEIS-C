/*
 * A001882  a(2n) = a(2n-1) + 2a(2n-2), a(2n+1) = a(2n) + a(2n-1), with a(1) =
 *          2 and a(2) = 3.
 * https://oeis.org/A001882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = a(2n-1) + 2*a(2n-2), a(2n+1) = a(2n) + a(2n-1), with
 *            a(1) = 2, a(2) = 3
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev2, prev1, cur;
    idx_t i;
    if (n == 1) return 2;
    if (n == 2) return 3;
    prev2 = 2;  /* a(i-2) */
    prev1 = 3;  /* a(i-1) */
    cur = prev1;
    for (i = 3; i <= n; i++) {
        if (i % 2 == 0)
            cur = prev1 + (seq_t)2 * prev2;
        else
            cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
