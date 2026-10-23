/*
 * A190826  Number of permutations of 3 copies of 1..n introduced in order
 *          1..n with no element equal to another within a distance of 1.
 * https://oeis.org/A190826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A190826: Number of permutations of 3 copies of 1..n... */
/* Recurrence (valid for n ≥ 4):                                       */
/*   2*a(n) -3*(3*n^2-3*n+4)*a(n-1) +2*(9*n^2-42*n+47)*a(n-2)         */
/*   +8*(3*n-7)*a(n-3) -8*a(n-4) = 0                                 */
#define A190826_ITERATIONS
seq_t a(idx_t n)
{
    /* Initial values for n = 0 .. 3 */
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)0;
    if (n == 2ULL) return (seq_t)1;
    if (n == 3ULL) return (seq_t)29;

    /* Variables to hold the last four terms */
    seq_t a0 = (seq_t)1;   /* a(k-4)   */
    seq_t a1 = (seq_t)0;   /* a(k-3)   */
    seq_t a2 = (seq_t)1;   /* a(k-2)   */
    seq_t a3 = (seq_t)29;  /* a(k-1)   */

    for (idx_t k = 4ULL; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;

        /* Compute the coefficients of the recurrence */
        seq_t c1 = (seq_t)3 *
                   (((seq_t)3 * kk * kk) - (seq_t)3 * kk + (seq_t)4);   /* 3*(3k^2-3k+4) */
        seq_t c2 = (seq_t)2 *
                   (((seq_t)9 * kk * kk) - (seq_t)42 * kk + (seq_t)47);  /* 2*(9k^2-42k+47) */
        seq_t c3 = (seq_t)8 *
                   (((seq_t)3 * kk) - (seq_t)7);                       /* 8*(3k-7) */

        /* Numerator of the recurrence */
        seq_t num = c1 * a3
                  - c2 * a2
                  - c3 * a1
                  + (seq_t)8 * a0;

        seq_t next = num / (seq_t)2;   /* divide by 2, as the recurrence has factor 2 */

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;   /* a(n) */
}
