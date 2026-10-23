/*
 * A172529  Number of ways to place 2 nonattacking knights on an n X n
 *          toroidal board.
 * https://oeis.org/A172529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n+3) * (n-3) / 2
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Predefined values for n < 5 */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)18;
    if (n == 4) return (seq_t)88;

    /* Cast to seq_t once for all integer arithmetic */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;          /* n^2          */
    seq_t B  = N + (seq_t)3;   /* n+3          */
    seq_t C  = N - (seq_t)3;   /* n-3          */

    /* One of the three factors is guaranteed to be even.
       Divide that factor by 2 first to keep intermediate values
       as small as possible and avoid overflows on 64‑bit builds. */
    if ((N2 & (seq_t)1) == 0) {
        N2 >>= 1;
    } else if ((B & (seq_t)1) == 0) {
        B >>= 1;
    } else {
        C >>= 1;
    }

    /* Compute the product after the early division. */
    seq_t res = N2 * B;
    res *= C;
    return res;
}
