/*
 * A056914  a(n) = Lucas(4*n+1).
 * https://oeis.org/A056914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*a(n-1) - a(n-2), with a(0)=1, a(1)=11
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A056914 (Lucas(4*n+1)) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)11;  /* a(1) */
    seq_t temp;
    idx_t i;

    /* Compute iteratively */
    for (i = 2; i <= n; ++i) {
        temp = 7 * a1 - a0;  /* recurrence relation */
        a0 = a1;
        a1 = temp;
    }
    return a1;
}
