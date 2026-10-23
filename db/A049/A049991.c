/*
 * A049991  a(n) is the number of arithmetic progressions of 2 or more
 *          positive integers, nondecreasing with sum <= n.
 * https://oeis.org/A049991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049991:  a(n) = number of arithmetic progressions of length >=2
   of positive integers, nondecreasing, with total sum <= n.
   The implementation counts (a,d,k) with 
        k >= 2, a >= 1, d >= 0, 
        k*a + d*k*(k-1)/2 <= n  */
/*  The algorithm runs in O(n sqrt n) time with ≤~3.3n inner iterations
    for n up to 10⁴. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if (N < 2) return 0;

    seq_t total = 0;
    for (idx_t kk = 2; (seq_t)kk <= N; ++kk)
    {
        seq_t k = (seq_t)kk;
        seq_t t = k * (k - 1) / 2;          /* triangular number t = k(k-1)/2 */

        /* maximum d such that k*a + d*t <= N with a>=1 =>
           N - d*t >= k  ==>  d <= floor((N - k)/t)  */
        seq_t dmax = ((seq_t)kk <= N) ? ((N - k) / t) : 0;

        for (seq_t d = 0; d <= dmax; ++d)
        {
            seq_t maxA = (N - d * t) / k;   /* a ranges from 1 .. maxA */
            if (maxA >= 1) total += maxA;
        }
    }
    return total;
}
