/*
 * A178218  Numbers of the form 2k^2-2k+1 or 2k^2-1.
 * https://oeis.org/A178218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(n+2) + 3*(-1)^n + 1) / 4 (equivalent recurrence:
 *            a(n) = n*(n+1) - a(n-1), with a(1)=1)
 * Range:     u8 n <= 10, u16 n <= 213, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* 2*n*(n+2) */
    seq_t term = (seq_t)2 * N * (N + (seq_t)2);

    /* add 3*(-1)^n + 1 :
       if n is even   (+3+1) = +4
       if n is odd    (-3+1) = -2 */
    if ((n & (idx_t)1ULL) == 0)
    {
        term += (seq_t)4;
    }
    else
    {
        term -= (seq_t)2;
    }

    return term / (seq_t)4;
}
