/*
 * A099603  Row sums of triangle A099602, in which row n equals the inverse
 *          binomial transform of column n of the triangle of trinomial
 *          coefficients (A027907).
 * https://oeis.org/A099603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+1) * 2^((n+1)/2)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);                 /* a = F(k), b = F(k+1) where k = n/2 */
    seq_t two_b = b << 1;                     /* 2*b */
    c = a * (two_b - a);                     /* F(2k)   */
    d = a * a + b * b;                       /* F(2k+1) */
    if ((n & 1) == 0) {                      /* n even  */
        *fn  = c;
        *fn1 = d;
    } else {                                 /* n odd   */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fib_n1, unused;
    fib_pair(n + 1, &fib_n1, &unused);   /* fib_n1 = F(n+1) */

    idx_t half = (n + 1) / 2;
    seq_t pow2 = 1;
    for (idx_t i = 0; i < half; ++i)
        pow2 <<= 1;                       /* 2^half */

    return fib_n1 * pow2;
}
