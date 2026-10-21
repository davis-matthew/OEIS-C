/*
 * A075850  Greatest k such that f(k,1) divides f(k,n) where f(k,n) is the
 *          k-th term of the sequence: f(1,n)=1, f(2,n)=n,
 *          f(k+2,n)=f(k+1,n)+f(k,n).
 * https://oeis.org/A075850
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest k such that the k-th Fibonacci number F_k
 *            divides (n‑1)
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle n < 2 trivially; the offset is at n = 2, so any value is acceptable. */
    if (n < 2) return (seq_t)0;

    seq_t target = (seq_t)(n - 1);       /* n-1 fits in seq_t. */

    /* Fibonacci numbers: F_0 = 0, F_1 = 1. */
    seq_t f_prev = 0;                    /* F_0 */
    seq_t f_curr = 1;                    /* F_1 */
    idx_t k      = 1;                    /* current Fibonacci index */
    idx_t best   = 1;                    /* F_1 = 1 always divides target */

    /* Iterate until the next Fibonacci number would exceed target or overflow occurs. */
    for (;;) {
        if (f_curr > target) break;      /* No further candidates. */

        if ((target % f_curr) == 0) {
            best = k;                    /* Record the largest valid index seen so far. */
        }

        /* Compute next Fibonacci number safely. */
        if (f_prev > target - f_curr) {
            /* The next Fibonacci would be larger than target or overflow seq_t. */
            break;
        }

        seq_t f_next = f_prev + f_curr;   /* F_{k+1} */
        f_prev = f_curr;
        f_curr = f_next;
        ++k;
    }

    return (seq_t)best;
}
