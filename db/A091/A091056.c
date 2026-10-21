/*
 * A091056  Expansion of x^2/((1-x)*(1+2*x)*(1-6*x)).
 * https://oeis.org/A091056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 8*a(n-2) - 12*a(n-3), with a(0)=0, a(1)=0,
 *            a(2)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A091056 */
seq_t a(idx_t n)
{
    /* Handle initial terms directly */
    if (n == 0 || n == 1) return 0;
    if (n == 2) return 1;

    /* Recurrence: a(n)=5*a(n-1)+8*a(n-2)-12*a(n-3) */
    seq_t a0 = 0; /* a(n-3) */
    seq_t a1 = 0; /* a(n-2) */
    seq_t a2 = 1; /* a(n-1) */
    seq_t a3;     /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute 5*a2 + 8*a1 - 12*a0  (all intermediate values are non‑negative) */
        seq_t term = 5 * a2 + 8 * a1;
        a3 = term - 12 * a0;

        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2; /* a(n) */
}
