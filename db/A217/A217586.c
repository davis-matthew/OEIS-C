/*
 * A217586  a(1) = 1 and, for all n >= 1, if a(n) = 0 then a(2*n) = 1 and
 *          a(2*n+1) = 0 whereas if a(n) = 1 then a(2*n) = 0 and a(2*n+1) = 0.
 * https://oeis.org/A217586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9132, u16 n <= 9132, u32 n <= 9132, u64 n <= 9132,
 *            u128 n <= 9132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   For n > 1, let p = a(n >> 1).  If p == 0, then a(n) = 1 when n is even, else 0.
   If p == 1, then a(n) = 0. */

/* Compute A217586 for index n (1‑based). */
seq_t a(idx_t n) {
    if (n == 1) {
        return 1;
    }
    idx_t parent = n >> 1;
    seq_t parent_val = a(parent);
    if (parent_val == 0) {
        /* parent was zero: even indices become 1, odd become 0 */
        return (n & 1) ? 0 : 1;
    }
    /* parent was one, all descendants are zero */
    return 0;
}
