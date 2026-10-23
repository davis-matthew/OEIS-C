/*
 * A192389  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*Fib(n+4) - n*(n+4) - 9
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast doubling Fibonacci: computes F(n) and F(n+1) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    idx_t k = n >> 1;
    fib_pair(k, &a, &b);                /* a = F(k), b = F(k+1) */

    /* F(2k) = a * (2*b - a) */
    seq_t two_b = b;
    two_b += b;                         /* 2*b */
    seq_t two_b_minus_a = two_b;
    two_b_minus_a -= a;                 /* 2*b - a */
    seq_t c = a * two_b_minus_a;        /* F(2k) */

    /* F(2k+1) = a^2 + b^2 */
    seq_t d = a * a;                    /* a^2 */
    seq_t e = b * b;                    /* b^2 */
    seq_t f = d + e;                    /* a^2 + b^2 */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = f;
    } else {
        *fn  = f;
        seq_t fn1_tmp = f;
        fn1_tmp += c;                   /* F(2k)+F(2k+1) = F(2k+2) */
        *fn1 = fn1_tmp;
    }
}

seq_t a(idx_t n)
{
    seq_t fib_n4;
    seq_t dummy;
    fib_pair(n + 4, &fib_n4, &dummy);   /* fib_n4 = Fib(n+4) */

    /* three times Fib(n+4) */
    seq_t three_fib = fib_n4;
    three_fib += fib_n4;
    three_fib += fib_n4;

    seq_t sn = (seq_t)n;
    seq_t prod = sn * (sn + (seq_t)4);

    seq_t result = three_fib;
    result -= prod;
    result -= (seq_t)9;
    return result;
}
