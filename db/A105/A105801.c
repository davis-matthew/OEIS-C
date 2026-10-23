/*
 * A105801  Fibonacci-Collatz sequence: a(1)=1, a(2)=2; for n > 2, let fib =
 *          a(n-1) + a(n-2); if fib is odd then a(n) = 3*fib + 1 else a(n) =
 *          fib/2.
 * https://oeis.org/A105801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n>2: fib = a(n-1)+a(n-2); if fib is odd then
 *            a(n)=3*fib+1 else a(n)=fib/2
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 58, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* undefined, but safe guard */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)2;   /* a(2) */
    seq_t cur = prev1;        /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t fib = prev1 + prev2;
        if ((fib & (seq_t)1) != (seq_t)0) {          /* odd */
            cur = fib * (seq_t)3 + (seq_t)1;
        } else {                                     /* even */
            cur = fib / (seq_t)2;
        }
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
