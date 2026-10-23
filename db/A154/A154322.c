/*
 * A154322  a(n) = 1 + n + binomial(n+3,5).
 * https://oeis.org/A154322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + n + binomial(n+3,5)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 162, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* convert index to seq_t */
    seq_t res = 1;               /* will hold binomial(n+3,5) */
    seq_t base = N + 3;          /* n+3 */

    /* compute C(n+3,5) multiplicatively: product_{k=1..5} (n+4-k)/k */
    for (int k = 1; k <= 5; ++k) {
        res = res * (base - (k - 1));
        res = res / (seq_t)k;    /* exact division */
    }

    return (seq_t)1 + N + res;   /* 1 + n + C(n+3,5) */
}
