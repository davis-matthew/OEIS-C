/*
 * A136807  Hankel transform of double factorial numbers n!*2^n=A000165(n).
 * https://oeis.org/A136807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=1..n} (2k)^(2(n-k+1))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence:  a(0) = 1
                            a(n) = a(n-1) * (2^n * n!)^2   for n ≥ 1  */

/* The function uses only integer operations and assumes that seq_t
   supports the usual arithmetic operators (or at least the overloads
   for `*` and assignment).  For arbitrary‑precision builds the provided
   helper `seq_mul` can be used if desired, but the operators are
   generally defined.  No dynamic memory allocation is required. */

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;   /* a(0) */
    seq_t fact   = (seq_t)1;   /* k!   */
    seq_t p4     = (seq_t)1;   /* 4^k  */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* update factorial and 4^k for this k */
        fact *= (seq_t)k;        /* fact = k!   */
        p4   *= (seq_t)4;        /* p4   = 4^k  */

        /* (k!)^2 */
        seq_t sq = fact * fact;

        /* result = result * 4^k * (k!)^2 */
        result *= p4;
        result *= sq;
    }

    return result;
}
