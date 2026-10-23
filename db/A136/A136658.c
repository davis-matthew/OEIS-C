/*
 * A136658  Row sums of unsigned triangle A136656 and also of triangle
 *          2*A136657.
 * https://oeis.org/A136658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n-1)*a(n-1) - 3*(n-2)*(n-1)*a(n-2) +
 *            (n-3)*(n-2)*(n-1)*a(n-3) with initial values a(0)=1, a(1)=2,
 *            a(2)=10
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) */
    if (n == 1) return (seq_t)2;   /* a(1) */
    if (n == 2) return (seq_t)10;  /* a(2) */

    /* store the last three terms: a(k-3), a(k-2), a(k-1) */
    seq_t a_kminus3 = (seq_t)1;      /* a(0) */
    seq_t a_kminus2 = (seq_t)2;      /* a(1) */
    seq_t a_kminus1 = (seq_t)10;     /* a(2) */
    seq_t a_k = 0;                   /* placeholder for a(k) */

    for (idx_t k = 3; k <= n; ++k)
    {
        seq_t k_s    = (seq_t)k;
        seq_t k_m1   = k_s - 1;          /* (n-1) */
        seq_t k_m2   = k_s - 2;          /* (n-2) */
        seq_t k_m3   = k_s - 3;          /* (n-3) */

        /* compute a(k) using the recurrence */
        a_k = ((seq_t)3 * k_s - 1) * a_kminus1
              - (seq_t)3 * k_m2 * k_m1 * a_kminus2
              + k_m3 * k_m2 * k_m1 * a_kminus3;

        /* shift the window for the next iteration */
        a_kminus3 = a_kminus2;
        a_kminus2 = a_kminus1;
        a_kminus1 = a_k;
    }

    return a_k;
}
