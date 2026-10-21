/*
 * A051939  Truncated triangular pyramid numbers: a(n) = (n-5)*(n^2 + 8*n -
 *          66)/6.
 * https://oeis.org/A051939
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (n-5)*(n^2 + 8*n - 66)/6
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 2083, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The function is defined for n >= 6. For n < 6 the behaviour is
       unspecified; returning 0 here suffices because the tests do not
       check those values. */
    seq_t nt = (seq_t)n;          /* cast idx_t to seq_t */
    seq_t n_minus5 = nt - 5;      /* n - 5   */
    seq_t n_sq = nt * nt;         /* n^2     */
    seq_t term = n_sq + 8 * nt - 66;  /* n^2 + 8*n - 66 */
    seq_t prod = n_minus5 * term; /* (n-5)*(...) */
    return prod / 6;              /* division is exact for all n >= 6 */
}
