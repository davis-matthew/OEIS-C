/*
 * A067332  Convolution of Fibonacci F(n+1), n>=0, with F(n+4), n>=0.
 * https://oeis.org/A067332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((11*n+15)*F(n+1)+7*(n+1)*F(n))/5, where F is the
 *            Fibonacci sequence with F(0)=0, F(1)=1
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t a, b;                 /* a = F(k), b = F(k+1), k = n/2 */
    fib_pair(n >> 1, &a, &b);

    seq_t two_b = b + b;              /* 2*b */
    seq_t two_b_minus_a = two_b - a;  /* 2*b - a */
    seq_t c = a * two_b_minus_a;      /* F(2k)   = a*(2*b - a) */
    seq_t a_sq = a * a;
    seq_t b_sq = b * b;
    seq_t d = a_sq + b_sq;            /* F(2k+1) = a^2 + b^2 */

    if ((n & 1) == 0) {
        *fn = c;
        *fn1 = d;
    } else {
        *fn = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t Fn, Fn1;
    fib_pair(n, &Fn, &Fn1);   /* Fn  = F(n)   , Fn1 = F(n+1) */

    seq_t nn = (seq_t)n;
    seq_t num1 = (seq_t)11 * nn + (seq_t)15;                /* 11*n+15 */
    seq_t num2 = (seq_t)7 * (nn + (seq_t)1);                /* 7*(n+1) */

    seq_t term1 = Fn1 * num1;
    seq_t term2 = Fn   * num2;

    seq_t sum = term1 + term2;
    return sum / (seq_t)5;   /* Division is exact */
}
