/*
 * A221364  The simple continued fraction expansion of F(x) := Product_{n >=
 *          0} (1 - x^(4*n+3))/(1 - x^(4*n+1)) when x = 1/2*(3 - sqrt(5)).
 * https://oeis.org/A221364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   a(0) = a(1) = a(2) = 1; a(3) = 5; a(4) = 1; a(5) = 16; a(n) =
 *            4*a(n-2) - 4*a(n-4) + a(n-6) for n >= 6
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 46, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0 || n == 1 || n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)1;
    if (n == 5) return (seq_t)16;

    /* sliding window for last 6 values a(n-6)..a(n-1) */
    seq_t v0 = (seq_t)1;   /* a(0) */
    seq_t v1 = (seq_t)1;   /* a(1) */
    seq_t v2 = (seq_t)1;   /* a(2) */
    seq_t v3 = (seq_t)5;   /* a(3) */
    seq_t v4 = (seq_t)1;   /* a(4) */
    seq_t v5 = (seq_t)16;  /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t nxt = (seq_t)4 * v4 - (seq_t)4 * v2 + v0;
        /* shift window */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = v5;
        v5 = nxt;
    }
    return v5;
}
