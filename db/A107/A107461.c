/*
 * A107461  Number of gap-free compositions of n into distinct parts, cf.
 *          A107428.
 * https://oeis.org/A107461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = Σ_{k≥1, n≥k(k+1)/2, (n−k(k+1)/2) mod k = 0} k!
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, for each k for which a set of k consecutive integers can sum to n,  */
/* all k! permutations of that set give distinct gap‑free compositions.        */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)0;
    seq_t factorial = (seq_t)1;    /* factorial of current k   */
    for (idx_t k = 1; ; ++k)
    {
        /* Update factorial (k! ) for this k.  */
        if (k > 1)
            factorial = factorial * (seq_t)k;

        seq_t min_sum = (seq_t)k * (k + 1) / 2;   /* minimum sum of k consecutive integers */
        if (min_sum > (seq_t)n)
            break;                                 /* no larger k can work */

        seq_t diff = (seq_t)n - min_sum;
        if (diff % (seq_t)k == 0)
            res += factorial;                         /* add permutations for this k */
    }
    return res;
}
