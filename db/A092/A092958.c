/*
 * A092958  a(1) = 1, a(2) = (1+2)*(2+3), a(3) = (1+2+3)*(2+3+4)*(3+4+5), ...
 *          etc. Or a(n) = (T(n))*(T(n)+n)*(T(n)+2n)*(T(n)+3n)*... n terms.
 *          where T(n) = n(n+1)/2 given by A000217.
 * https://oeis.org/A092958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{k=0}^{n-1} ( n(n+1)/2 + k·n ) (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* index < 1 is unused but harmless   */
    seq_t N = (seq_t)n;            /* convert index to seq_t for safety  */
    seq_t T = N * (N + 1) / 2;     /* triangular number T(n)             */
    seq_t result = 1;              /* running product                    */
    for (idx_t k = 0; k < n; ++k)
        result *= T + N * (seq_t)k;/* multiply by each term T + k·n      */
    return result;
}
