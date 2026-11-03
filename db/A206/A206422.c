/*
 * A206422  Fibonacci sequence beginning 11, 9.
 * https://oeis.org/A206422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*Fibonacci(n) - 2*Fibonacci(n-1); a(1)=11, a(2)=9;
 *            a(n)=a(n-1)+a(n-2) for n>2
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 922 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair_recursive(idx_t n, seq_t *f_n, seq_t *f_n1)
{
    if (n == 0) {
        *f_n = 0;
        *f_n1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair_recursive(n >> 1, &a, &b);
    /* c = F(2k), d = F(2k+1) for k = n>>1 */
    seq_t c = a * (((b << 1) - a));
    seq_t d = a * a + b * b;
    if (n & 1) {
        *f_n = d;          /* F(2k+1) */
        *f_n1 = c + d;     /* F(2k+2) */
    } else {
        *f_n = c;          /* F(2k)   */
        *f_n1 = d;         /* F(2k+1) */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but safe source */

    seq_t fn, fn1;
    fib_pair_recursive(n, &fn, &fn1);   /* fn  = Fib(n),  fn1 = Fib(n+1) */
    seq_t fn_minus1 = fn1 - fn;          /* Fib(n-1) = Fib(n+1) - Fib(n) */

    return 11 * fn - 2 * fn_minus1;
}
