/*
 * A089941  Row sums of triangle A089940.
 * https://oeis.org/A089941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} binomial(n+k, floor((n-k)/2))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 17, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (valid for n ≥ 6):
     n*(n+6)*(n-5)*a(n)
   = (n+6)*(n^2-3*n-6)*a(n-1)
   + 2*(n+6)*(3*n^2-20*n+27)*a(n-2)
   - (n+6)*(3*n^2-19*n+36)*a(n-3)
   - 2*(4*n-15)*(n-3)*(n+6)*a(n-4)
   - 4*(n-3)*(n-4)*(n+6)*a(n-5)  */

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Base values for n = 0..5. */
    static const seq_t base[6] = {1, 2, 4, 9, 19, 42};

    if (n < 6)
        return base[n];

    /* Initialize the last five terms: a(n-5) … a(n-1). */
    seq_t b0 = base[1];  /* a(1) */
    seq_t b1 = base[2];  /* a(2) */
    seq_t b2 = base[3];  /* a(3) */
    seq_t b3 = base[4];  /* a(4) */
    seq_t b4 = base[5];  /* a(5) */

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t N = (seq_t)i;

        seq_t termA = (N + 6) * (N * N - 3 * N - 6) * b4;
        seq_t termB = 2 * (N + 6) * (3 * N * N - 20 * N + 27) * b3;
        seq_t termC = (N + 6) * (3 * N * N - 19 * N + 36) * b2;
        seq_t termD = 2 * (4 * N - 15) * (seq_t)(i - 3) * (N + 6) * b1;
        seq_t termE = 4 * (seq_t)(i - 3) * (seq_t)(i - 4) * (N + 6) * b0;

        seq_t numerator   = termA + termB - termC - termD - termE;
        seq_t denominator = N * (N + 6) * (N - 5);

        seq_t ai = numerator / denominator;

        /* Shift the history for the next iteration. */
        b0 = b1;
        b1 = b2;
        b2 = b3;
        b3 = b4;
        b4 = ai;
    }

    /* After the loop, b4 holds a(n). */
    return b4;
}
