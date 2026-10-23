/*
 * A177188  Quotients of A024167(4n) and 6n+1.
 * https://oeis.org/A177188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A024167(4n) / (6n+1) where A024167(m) = m! *
 *            sum_{k=1}^{m} (-1)^{k+1} / k The recurrence A(0) = 0 A(m) =
 *            m*A(m-1) + (-1)^{m+1} * (m-1)! is used to build A(4n). The
 *            result is divided by (6n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fact = 1;        /* (i-1)!  at the start of each iteration   */
    seq_t A    = 0;        /* A(m) for the current m                    */

    idx_t limit = 4 * n;   /* number of iterations                       */

    for (idx_t i = 1; i <= limit; ++i) {
        seq_t term = fact;                /* (i-1)!                          */
        if (i & 1U) {                    /* odd i : sign +1                    */
            A = (seq_t)i * A + term;
        } else {                         /* even i : sign -1                   */
            A = (seq_t)i * A - term;
        }
        fact *= i;                       /* update factorial for next i         */
    }

    seq_t denom = (seq_t)n * 6 + 1;      /* 6n+1                               */
    return A / denom;
}
