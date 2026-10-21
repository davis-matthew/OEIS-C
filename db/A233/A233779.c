/*
 * A233779  Main diagonal of A234951.
 * https://oeis.org/A233779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n*(n+1)/2 - floor((n-1)*(n-2)/4) for n ≥ 4
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (values for n = 1,2,3 are 1,3,5 respectively) */
seq_t a(idx_t n)
{
    /* Handle the small n explicitly to satisfy the given terms. */
    if (n == 0)
        return 0;   /* offset starts at 1; this case is unused but defined for safety */
    if (n == 1)
        return 1;
    if (n == 2)
        return 3;
    if (n == 3)
        return 5;

    /* Convert to seq_t first to keep all computations in the result type. */
    seq_t N = (seq_t)n;

    /* Triangular number T(n) = n*(n+1)/2 */
    seq_t T = N * (N + 1) / 2;

    /* Subtract floor((n-1)*(n-2)/4). Integer division automatically truncates. */
    seq_t sub = ((N - 1) * (N - 2)) / 4;

    return T - sub;
}
