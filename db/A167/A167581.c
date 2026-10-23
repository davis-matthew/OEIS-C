/*
 * A167581  The second left hand column of triangle A167580.
 * https://oeis.org/A167581
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2^n * (2*n^3 - 9*n^2 + 10*n - 3) / 12, n ≥ 2
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 20, i64 n <= 48, i128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for n < 2 – return 0 for safety */
    if (n < 2) return 0;

    /* poly = 2*n^3 - 9*n^2 + 10*n - 3  using only seq_t ops */
    seq_t N   = (seq_t)n;
    seq_t N2  = N * N;          /* n^2   */
    seq_t N3  = N2 * N;         /* n^3   */
    seq_t poly = 2 * N3 - 9 * N2 + 10 * N - 3;

    /* the whole expression is divisible by 12 = 3 * 4;
       we first cancel the factor 3 to keep numbers small       */
    seq_t part = poly / 3;      /* integer division, no remainder */

    /* multiply by 2^(n-2); do it one step at a time to avoid
       intermediate overflow when the final result fits in seq_t   */
    for (idx_t i = 0; i < n - 2; ++i)
        part *= 2;

    return part;
}
