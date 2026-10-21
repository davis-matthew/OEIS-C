/*
 * A233123  Number of n X 2 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally or vertically, top left
 *          element zero, and 1 appearing before 2 3 and 4, and 2 appearing
 *          before 3 in row major order (unlabelled 6-colorings with no
 *          clashing color pairs).
 * https://oeis.org/A233123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 48*a(n-2) Initial values: a(1) = 1, a(2) = 8
 *            Recurrence holds for all n ≥ 3. The implementation is an O(n)
 *            iterative computation using the recurrence. It works for any
 *            seq_t type
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the offseted index (a(1) is first term) */
    if (n == 0) return 0;          /* undefined, but return 0 for safety */
    if (n == 1) return 1;
    if (n == 2) return 8;

    seq_t prev2 = 1; /* a(1) */
    seq_t prev1 = 8; /* a(2) */
    seq_t curr  = 0;

    for (idx_t i = 3; i <= n; ++i) {
        curr = 16 * prev1 - 48 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
