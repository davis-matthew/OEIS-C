/*
 * A206208  Number of (n+1) X 2 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two equal edges, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A206208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=14, a(2)=45, a(3)=162, a(4)=594, a(5)=2268 a(n) = 4*a(n-1)
 *            - a(n-2) + 12*a(n-3) - 36*a(n-4) for n > 5 (indices are 1‑based,
 *            a(0) returns 0) The function computes the value by iterating the
 *            recurrence; no dynamic allocation is required. The code is valid
 *            C11 and C++17
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                     /* unused offset */

    if (n == (idx_t)1) return (seq_t)14;
    if (n == (idx_t)2) return (seq_t)45;
    if (n == (idx_t)3) return (seq_t)162;
    if (n == (idx_t)4) return (seq_t)594;
    if (n == (idx_t)5) return (seq_t)2268;

    /* variables hold a(n-5) … a(n-1) for the current iteration
       start with a(1)…a(5) */
    seq_t a1 = (seq_t)14;   /* a(n-5) */
    seq_t a2 = (seq_t)45;   /* a(n-4) */
    seq_t a3 = (seq_t)162;  /* a(n-3) */
    seq_t a4 = (seq_t)594;  /* a(n-2) */
    seq_t a5 = (seq_t)2268; /* a(n-1) */

    for (idx_t i = (idx_t)6; i <= n; ++i) {
        /* Compute a(i) using the recurrence */
        seq_t next = (seq_t)4 * a5;          /* 4*a(i-1) */
        next -= a4;                         /* - a(i-2) */
        next += (seq_t)12 * a3;             /* + 12*a(i-3) */
        next -= (seq_t)36 * a2;             /* - 36*a(i-4) */

        /* Slide window: drop the oldest, shift, and add new */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5; /* a(n) */
}
