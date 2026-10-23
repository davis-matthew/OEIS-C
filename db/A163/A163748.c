/*
 * A163748  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A163748   Number of reduced words
 *
 *  Recurrence (derived from the generating function):
 *
 *      a(0) = 1
 *      a(1) = 44
 *      a(2) = 1892
 *      a(3) = 81356
 *      a(4) = 3498308
 *
 *      a(5) = 42*a(4)+42*a(3)+42*a(2)+42*a(1)-903*a(0)+1
 *
 *      a(n) = 42*a(n-1)+42*a(n-2)+42*a(n-3)+42*a(n-4)-903*a(n-5)
 *            for all n ≥ 6
 *
 *  The function below computes a(n) for arbitrary non‑negative n,
 *  using only integer arithmetic allowed for ‘seq_t’ and ‘idx_t’
 *  and works for any integer type chosen by the build system.
 */
 
seq_t a(idx_t n)
{
    /* Initial known values */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)44;
    if (n == 2u) return (seq_t)1892;
    if (n == 3u) return (seq_t)81356;
    if (n == 4u) return (seq_t)3498308;

    /* Sliding window for the last five sequence values:
     * t4 = a(i-1), t3 = a(i-2), t2 = a(i-3), t1 = a(i-4), t0 = a(i-5)
     */
    seq_t t4 = (seq_t)3498308; /* a(4)   */
    seq_t t3 = (seq_t)81356;   /* a(3)   */
    seq_t t2 = (seq_t)1892;    /* a(2)   */
    seq_t t1 = (seq_t)44;      /* a(1)   */
    seq_t t0 = (seq_t)1;       /* a(0)   */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t sum = (seq_t)42 * t4
                  + (seq_t)42 * t3
                  + (seq_t)42 * t2
                  + (seq_t)42 * t1;

        seq_t next = sum - (seq_t)903 * t0;
        if (i == 5u)                 /* constant term for a(5) */
            next += (seq_t)1;

        /* Shift the window for the next iteration */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = next;
    }
    return t4;
}
