/*
 * A162960  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A162960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 7*a(n-1) + 7*a(n-2)
 *            + 7*a(n-3) - 28*a(n-4) + [n==4] with initial values: a(0)=1,
 *            a(1)=9, a(2)=72, a(3)=576, a(4)=4572. Derived from the
 *            generating function: (t^4 + 2*t^3 + 2*t^2 + 2*t + 1)/(28*t^4 -
 *            7*t^3 - 7*t^2 - 7*t + 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A162960.
   n is an uint64_t index (idx_t). The return type seq_t
   is an unsigned integer type (possibly arbitrary precision). */
seq_t a(idx_t n)
{
    /* Handle the initial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)72;
    if (n == 3) return (seq_t)576;

    /* Use a rolling window of the last four values */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)9;   /* a(n-3) */
    seq_t a2 = (seq_t)72;  /* a(n-2) */
    seq_t a3 = (seq_t)576; /* a(n-1) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) using the recurrence */
        cur = a3 * (seq_t)7
            + a2 * (seq_t)7
            + a1 * (seq_t)7
            - a0 * (seq_t)28;

        /* Add the numerator coefficient when i == 4 */
        if (i == 4)
            cur = cur + (seq_t)1;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }

    /* a3 now holds a(n) */
    return a3;
}
