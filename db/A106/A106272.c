/*
 * A106272  Antidiagonal sums of number triangle A106270.
 * https://oeis.org/A106272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=a(2)=-1, a(3)=-6.
   For n≥4: (n+1)*a(n) + 2*(1-2*n)*a(n-1) - (n+1)*a(n-2) + 2*(2*n-1)*a(n-3) = 0
   This function evaluates the sequence A106272 using that recurrence. */

seq_t a(idx_t n) {
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)-1;
    if (n == 3) return (seq_t)-6;

    /* Keep the last four values: a(n-3), a(n-2), a(n-1), a(n). */
    seq_t past[4];
    past[0] = (seq_t)1;      /* a(0) */
    past[1] = (seq_t)-1;     /* a(1) */
    past[2] = (seq_t)-1;     /* a(2) */
    past[3] = (seq_t)-6;     /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) from the recurrence. */
        seq_t N = (seq_t)(i + 1);                     /* (n+1)  */
        seq_t two_two_n_minus1 = (seq_t)(2 * ((seq_t)(2 * (seq_t)i - 1)));
        /* (n+1)*a(n-2) + 2*(2n-1)*(a(n-1) - a(n-3)) */
        seq_t termA = N * past[2];                    /* (n+1)*a(n-2) */
        seq_t termB = two_two_n_minus1 * (past[3] - past[1]); /* 2*(2n-1)*(a(n-1)-a(n-3)) */
        seq_t numerator = termA + termB;              /* RHS of the recurrence */
        seq_t cur = numerator / N;                    /* a(n) = numerator / (n+1) */

        /* Shift the window: a(n-3) <- a(n-2), a(n-2) <- a(n-1),
           a(n-1) <- a(n),  a(n)   <- cur          */
        past[0] = past[1];
        past[1] = past[2];
        past[2] = past[3];
        past[3] = cur;
    }

    return past[3];
}
