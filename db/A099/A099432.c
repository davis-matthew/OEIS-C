/*
 * A099432  Convolution of A030195(n) (generalized (3,3)-Fibonacci) with
 *          itself.
 * https://oeis.org/A099432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2) - 18*a(n-3) - 9*a(n-4) with a(0)=1,
 *            a(1)=6, a(2)=33, a(3)=162
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A099432.  The calculation uses only
   the recurrence relation, so it works for all integer types that
   support the usual arithmetic operators on seq_t. */
seq_t a(idx_t n)
{
    /* Special‑case the first four values – they are known explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)33;
    if (n == 3) return (seq_t)162;

    /* Recurrence needs the four previous values. */
    seq_t a0 = (seq_t)1;          /* a(n-4) */
    seq_t a1 = (seq_t)6;          /* a(n-3) */
    seq_t a2 = (seq_t)33;         /* a(n-2) */
    seq_t a3 = (seq_t)162;        /* a(n-1) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)6 * a3
            - (seq_t)3 * a2
            - (seq_t)18 * a1
            - (seq_t)9 * a0;

        /* Shift the window of previous terms. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
