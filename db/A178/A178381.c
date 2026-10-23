/*
 * A178381  Number of paths of length n starting at initial node of the path
 *          graph P_9.
 * https://oeis.org/A178381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-2) - 5*a(n-4) for n ≥ 5 Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=6
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 70, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast return for the first five terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)6;

    /* Circular buffer holding the last five computed terms.
       arr[0] = a(i-4), arr[1] = a(i-3), arr[2] = a(i-2),
       arr[3] = a(i-1), arr[4] = a(i) (current) */
    seq_t arr[5];
    arr[0] = (seq_t)1;  /* a(0) */
    arr[1] = (seq_t)1;  /* a(1) */
    arr[2] = (seq_t)2;  /* a(2) */
    arr[3] = (seq_t)3;  /* a(3) */
    arr[4] = (seq_t)6;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        /* Compute a(i) = 5*a(i-2) - 5*a(i-4) */
        seq_t term = (seq_t)5 * arr[(i - 2) % 5] - (seq_t)5 * arr[(i - 4) % 5];
        arr[i % 5] = term;
    }

    return arr[n % 5];
}
