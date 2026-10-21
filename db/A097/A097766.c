/*
 * A097766  Pell equation solutions (11*a(n))^2 - 122*b(n)^2 = -1 with
 *          b(n):=A097767(n), n >= 0.
 * https://oeis.org/A097766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 486*a(n-1) – a(n-2) (a(0) = 1, a(1) = 487)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence is evaluated iteratively, which works for any seq_t type,
   including built‑in unsigned integers and arbitrary‑precision big integer
   classes.  Only the two previous values are kept, so the computation
   is O(n) time and O(1) extra space. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)487;

    seq_t prev2 = (seq_t)1;   /* a(n‑2) */
    seq_t prev1 = (seq_t)487; /* a(n‑1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (prev1 * (seq_t)486) - prev2; /* a(i)  */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
