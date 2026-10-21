/*
 * A200539  Product of Fibonacci and Motzkin numbers: a(n) =
 *          A000045(n+1)*A001006(n).
 * https://oeis.org/A200539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+1) * Motzkin(n) * Fibonacci: F1 = 1, F2 = 1,
 *            F(k+2) = F(k+1) + F(k) * Motzkin: M0 = 1, M1 = 1, M(k) =
 *            ((2k+1)M(k-1)+(3k-3)M(k-2))/(k+2) for k ≥ 2 * The function
 *            computes the terms iteratively and works for any * build where
 *            seq_t supports the usual arithmetic operators
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases first */
    if (n == 0) {            /* Fibonacci(1)=1, Motzkin(0)=1 */
        return (seq_t)1 * (seq_t)1;
    }
    if (n == 1) {            /* Fibonacci(2)=1, Motzkin(1)=1 */
        return (seq_t)1 * (seq_t)1;
    }

    /* Initialise Fibonacci sequence: F1=1, F2=1 */
    seq_t fibPrev = (seq_t)1;  /* F(k-1)   */
    seq_t fibCurr = (seq_t)1;  /* F(k)     */

    /* Initialise Motzkin sequence: M0=1, M1=1 */
    seq_t motPrev = (seq_t)1;  /* M(k-1)   */
    seq_t motCurr = (seq_t)1;  /* M(k)     */

    /* Iterate from k = 2 up to n */
    for (idx_t k = 2; k <= n; ++k) {
        /* Fibonacci update: F(k+1) = F(k) + F(k-1) */
        seq_t fibNext = fibCurr + fibPrev;
        fibPrev = fibCurr;
        fibCurr = fibNext;

        /* Motzkin update: M(k) = ((2k+1)M(k-1)+(3k-3)M(k-2))/(k+2) */
        seq_t num = ((seq_t)(2 * k + 1)) * motCurr + ((seq_t)(3 * k - 3)) * motPrev;
        seq_t denom = (seq_t)(k + 2);
        seq_t motNext = num / denom;   /* division is exact by definition */
        motPrev = motCurr;
        motCurr = motNext;
    }

    /* After the loop, fibCurr = Fibonacci(n+1), motCurr = Motzkin(n) */
    return fibCurr * motCurr;
}
