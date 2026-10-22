/*
 * A006904  a(n) = a(n-1) + 2*a(n-2) + (-1)^n.
 * https://oeis.org/A006904
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + (-1)^n, a(4) = 1, a(5) = 1
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev2 = 1; /* a(4) */
    seq_t prev1 = 1; /* a(5) */
    idx_t k;
    if (n <= 4) return prev2;
    if (n == 5) return prev1;
    for (k = 6; k <= n; k++) {
        seq_t cur = prev1 + 2 * prev2;
        if (k % 2 == 0) cur += 1;
        else cur -= 1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
