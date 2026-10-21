/*
 * A068922  Number of ways to tile a 3 X 2n room with 1 X 2 Tatami mats. At
 *          most 3 Tatami mats may meet at a point.
 * https://oeis.org/A068922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=3; for n≥2, a(n)=2*F(n+1) where F is Fibonacci
 *            (F(1)=1,F(2)=1). Recurrence: for n≥4, a(n)=a(n-1)+a(n-2) with
 *            a(1)=3,a(2)=4,a(3)=6
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Off‑by‑units handling: a(1) is special. */
    if (n == 0) return 0;       /* value below offset – not used */
    if (n == 1) return 3;       /* first listed term */

    /* For n ≥ 2, we need 2·F(n+1).  Compute Fibonacci iteratively. */
    idx_t k = n + 1;            /* target Fibonacci index (≥3) */
    seq_t prev = 1;             /* F(1) */
    seq_t curr = 1;             /* F(2) */

    for (idx_t i = 3; i <= k; ++i) {
        seq_t next = prev + curr; /* F(i) = F(i-1)+F(i-2) */
        prev = curr;
        curr = next;
    }

    return curr + curr;        /* 2 * F(k) = 2 * F(n+1) */
}
