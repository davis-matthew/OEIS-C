/*
 * A208089  Number of 8 X (n+1) 0..1 arrays with every 2 X 2 subblock having
 *          the same number of equal edges as its horizontal neighbors and a
 *          different number from its vertical neighbors, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A208089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 216 (base cases) a(2) = 504 a(3) = 1206 a(4) = 3096 For n
 *            ≥ 5, a(n) = 3*a(n-1) - 3*a(n-3) + a(n-4) (recurrence)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the first four values directly */
    if (n == 1) return (seq_t)216;
    if (n == 2) return (seq_t)504;
    if (n == 3) return (seq_t)1206;
    if (n == 4) return (seq_t)3096;

    /* Use a 4‑term sliding window to avoid dynamic allocation */
    seq_t a1 = (seq_t)216;   /* a(n-4) when n = 5 */
    seq_t a2 = (seq_t)504;   /* a(n-3) when n = 5 */
    seq_t a3 = (seq_t)1206;  /* a(n-2) when n = 5 */
    seq_t a4 = (seq_t)3096;  /* a(n-1) when n = 5 */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = a4 * (seq_t)3  /* 3*a(n-1) */
                    - a2 * (seq_t)3  /* 3*a(n-3) */
                    + a1;            /* + a(n-4) */

        /* shift the window forward for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    /* After the loop, a4 holds a(n) */
    return a4;
}
