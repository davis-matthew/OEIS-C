/*
 * A133471  a(n) = (n^2)*a(n-1) + a(n-2).
 * https://oeis.org/A133471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1; a(n) = n^2*a(n-1)+a(n-2) for n>=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0; /* a(0) */
    seq_t prev1 = 1; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii  = (seq_t)i;      /* convert index to seq_t */
        seq_t sq  = ii * ii;       /* n^2 */
        seq_t prod = sq * prev1;   /* n^2 * a(n-1) */
        seq_t curr = prod + prev2; /* a(n) = n^2*a(n-1)+a(n-2) */

        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
