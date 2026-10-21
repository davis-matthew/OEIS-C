/*
 * A097771  Chebyshev U(n,x) polynomial evaluated at x=339=2*13^2+1.
 * https://oeis.org/A097771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(-1)=0, a(n)=678*a(n-1)-a(n-2) for n>=1 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t prev = 0;      /* a(-1) */
    seq_t cur  = 1;      /* a(0)  */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t next = 678 * cur - prev;
        prev = cur;
        cur  = next;
    }
    return cur;
}
