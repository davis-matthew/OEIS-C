/*
 * A129395  Row sums of A129394.
 * https://oeis.org/A129395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0, a(1)=4, a(2)=-14, a(3)=32, a(4)=6, a(5)=-220. For n>=6:
 *            a(n) = -(4*a(n-1) + 6*a(n-2) - 4*a(n-3) - 15*a(n-4) - 8*a(n-5) +
 *            16*a(n-6))
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 26, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the recurrence follows from the rational generating function
   G(x) = -2*x*(4*x^4-17*x^3-x-2) / ((x-1)^2*(4*x^2+3*x+1)^2) */

static const seq_t initial[6] = { (seq_t)0, (seq_t)4, (seq_t)-14,
                                  (seq_t)32, (seq_t)6, (seq_t)-220 };

seq_t a(idx_t n) {
    if (n < 6) return initial[n];

    /* Keep the last six values in a circular buffer */
    seq_t prev[6] = { initial[0], initial[1], initial[2],
                      initial[3], initial[4], initial[5] };

    for (idx_t i = 6; i <= n; ++i) {
        seq_t sum = 4 * prev[5]            /* 4*a(n-1) */
                 + 6 * prev[4]            /* 6*a(n-2) */
                 - 4 * prev[3]            /* -4*a(n-3) */
                 - 15 * prev[2]           /* -15*a(n-4) */
                 - 8 * prev[1]            /* -8*a(n-5) */
                 + 16 * prev[0];          /* 16*a(n-6) */
        seq_t next = -sum;                 /* apply leading minus */

        /* shift buffer left */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = next;
    }
    return prev[5];
}
