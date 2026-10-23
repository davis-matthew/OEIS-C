/*
 * A186541  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=-2+3j^2. Complement
 *          of A186542.
 * https://oeis.org/A186541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt((1/3)*n^2+5/6))
 * Range:     u8 n <= 11, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic. */
    seq_t n_seq = (seq_t)n;
    /* Compute M = 2*n^2 + 5.  This is exactly 2*n^2 + 5. */
    seq_t M = 2 * n_seq * n_seq + (seq_t)5;

    /* Binary search for the largest integer A such that 6*A^2 <= M.
       Since A <= n (as shown above), we can restrict the search to [0, n]. */
    idx_t low = 0;
    idx_t high = n;   /* safe upper bound for A */

    while (low < high)
    {
        /* Use (low+high+1)/2 to avoid infinite loop when low+1==high. */
        idx_t mid = (low + high + 1) / 2;
        seq_t mid_sq = (seq_t)mid * (seq_t)mid;
        if (6 * mid_sq <= M)
            low = mid;   /* mid is feasible, search higher half */
        else
            high = mid - 1;  /* mid too large, search lower half */
    }

    seq_t A = (seq_t)low;
    return n_seq + A;
}
