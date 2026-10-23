/*
 * A049440  a(n) = Fib(3*n)^2 - 2*Fib(3*n) + 4*Fib(3*n+1) + 5, where Fib =
 *          A000045.
 * https://oeis.org/A049440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(3*n)^2 - 2*Fib(3*n) + 4*Fib(3*n+1) + 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);
    seq_t two_b = b * (seq_t)2;
    seq_t c = a * (two_b - a);
    seq_t d = a * a + b * b;
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    idx_t k = n * (idx_t)3;
    seq_t f3n, f3n1;
    fib_pair(k, &f3n, &f3n1);

    seq_t sq  = f3n * f3n;
    seq_t res = sq - (f3n * (seq_t)2) + (f3n1 * (seq_t)4) + (seq_t)5;
    return res;
}
