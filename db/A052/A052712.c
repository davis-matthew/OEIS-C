/*
 * A052712  Expansion of e.g.f. (1 + 4*x - sqrt(1-8*x))/8.
 * https://oeis.org/A052712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+1) = 4*(2*n-1)*a(n), a(0)=0, a(1)=1, a(2)=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Compute a(n) by iterating the recurrence. */
    seq_t result = (seq_t)2;        /* a(2) */
    for (idx_t i = 2; i < n; ++i)
    {
        /* factor = 4 * (2*i - 1) */
        seq_t factor = (seq_t)4 *
                       (((seq_t)2 * (seq_t)i) - (seq_t)1);
        result = result * factor;
    }
    return result;
}
