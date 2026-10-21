/*
 * A066769  a(n) = Sum_{d|n} d*Fibonacci(n/d).
 * https://oeis.org/A066769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} d*Fibonacci(n/d)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Fibonacci using fast doubling.  The helper returns F(n) and F(n+1). */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);           /* a = F(k), b = F(k+1), k = n/2 */

    /* Compute F(2k) = a * (2*b – a) and F(2k+1) = a*a + b*b */
    seq_t two_b_minus_a = ((seq_t)2 * b) - a;
    c = a * two_b_minus_a;
    d = a * a + b * b;

    if ((n & 1) == 0) {                 /* n is even */
        *fn  = c;
        *fn1 = d;
    } else {                            /* n is odd */
        *fn  = d;
        *fn1 = c + d;
    }
}

static inline seq_t fib(idx_t n)
{
    seq_t f, _;
    fib_pair(n, &f, &_);
    return f;
}

/* Main function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0;         /* offset is 1, but guard */

    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0U) continue;

        idx_t q = n / d;                  /* q = n/d */
        seq_t fq = fib(q);                 /* Fibonacci(q) = F(n/d) */
        sum += (seq_t)d * fq;              /* term for divisor d */

        if (d != q) {                     /* paired divisor */
            seq_t fd = fib(d);              /* Fibonacci(d) = F(n/q) */
            sum += (seq_t)q * fd;          /* term for divisor q */
        }
    }
    return sum;
}
