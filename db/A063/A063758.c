/*
 * A063758  a(0)=1, a(n) = 2*Fibonacci(n+4) - 6.
 * https://oeis.org/A063758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Fib(n+4) - 6 (with a(0) = 1)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    /* returns F(n) in *fn and F(n+1) in *fn1 using fast doubling */
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t a, b;           /* a = F(k), b = F(k+1)  where k = n/2 */
    fib_pair(n >> 1, &a, &b);

    /* c = F(2k)   = a * (2*b - a) */
    seq_t two_b_minus_a = b << 1;   /* 2*b   */
    two_b_minus_a      -= a;        /* 2*b - a */
    seq_t c = a * two_b_minus_a;

    /* d = F(2k+1) = a*a + b*b */
    seq_t d = a * a;
    d += b * b;

    if ((n & 1) == 0) {                /* even n = 2k */
        *fn  = c;
        *fn1 = d;
    } else {                           /* odd n = 2k+1 */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t fib_n_plus_4;
    seq_t unused;
    fib_pair(n + 4, &fib_n_plus_4, &unused);

    seq_t twice = fib_n_plus_4 << 1;   /* 2 * Fib(n+4) */
    return twice - (seq_t)6;
}
