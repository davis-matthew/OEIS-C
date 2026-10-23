/*
 * A022403  a(0)=a(1)=3; thereafter a(n) = a(n-1) + a(n-2) + 1.
 * https://oeis.org/A022403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Fibonacci(n+1) - 1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + 1, with a(0) = 3, a(1) = 3 */

static void fib_pair_rec(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t a, b;
    fib_pair_rec(n >> 1, &a, &b);          /* a = F(k), b = F(k+1), k = n/2 */

    /* Compute F(2k) and F(2k+1) */
    seq_t temp = (seq_t)2 * b;
    temp -= a;                             /* temp = 2*F(k+1) - F(k)  */
    seq_t c = a * temp;                    /* F(2k) = F(k) * temp   */
    seq_t d = a * a + b * b;               /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if (n & 1) {                           /* n is odd */
        *fn  = d;                           /* F(n)   = F(2k+1)       */
        *fn1 = c + d;                       /* F(n+1) = F(2k) + F(2k+1) */
    } else {                               /* n is even */
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    seq_t f_n, f_n1;
    fib_pair_rec(n + 1, &f_n, &f_n1);      /* f_n = Fibonacci(n+1)    */
    return f_n * (seq_t)4 - (seq_t)1;      /* a(n) = 4*Fib(n+1)-1     */
}
