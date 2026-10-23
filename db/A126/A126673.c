/*
 * A126673  Third diagonal of A126671.
 * https://oeis.org/A126673
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n*(n-5)/4 + 1 + 1/2 + ... + 1/n) Recurrence: a(2) =
 *            0 For n ≥ 3: a(n) = n * a(n-1) + (n-1)! * (n-1)*(n-2)/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    if (n == 2) return 0;

    seq_t fact = 1;   /* (i-1)! for current i in loop */
    seq_t val  = 0;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        fact = fact * (i - 1);                      /* fact = (i-1)!    */
        seq_t term = fact * (i - 1);                /* term = (i-1)!*(i-1)  */
        term = term * (i - 2) / 2;                  /* term = (i-1)!*(i-1)*(i-2)/2 */
        val = i * val + term;                       /* a(i) = i*a(i-1)+term  */
    }
    return val;
}
