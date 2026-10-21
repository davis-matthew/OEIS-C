/*
 * A083564  a(n) = L(n)*L(2n), where L(n) are the Lucas numbers (A000204).
 * https://oeis.org/A083564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L(n) * L(2n), where L(k) denotes the k‑th Lucas number
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* fast doubling algorithm for Fibonacci numbers: returns F(n) and F(n+1) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* (F(k), F(k+1)) for k = n/2          */
    seq_t two_b = (seq_t)2 * b;        /* 2*F(k+1)                          */
    seq_t c = a * (two_b - a);         /* F(2k) = F(k) * (2*F(k+1) - F(k))  */
    seq_t d = a * a + b * b;           /* F(2k+1) = F(k)^2 + F(k+1)^2       */
    if ((n & 1) == 0) {
        *fn  = c;  /* F(2k)        */
        *fn1 = d;  /* F(2k+1)      */
    } else {
        *fn  = d;         /* F(2k+1)       */
        *fn1 = c + d;     /* F(2k+2) = F(2k) + F(2k+1) */
    }
}

/* single Fibonacci value */
static inline seq_t fib(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);
    return fn;
}

seq_t a(idx_t n)
{
    /* Lucas number L(n) = F(n-1) + F(n+1) */
    seq_t f_n_minus1 = fib(n - 1);
    seq_t f_n_plus1  = fib(n + 1);
    seq_t Ln = f_n_minus1 + f_n_plus1;

    /* Lucas number L(2n) = F(2n-1) + F(2n+1) */
    seq_t f_2n_minus1 = fib(2 * n - 1);
    seq_t f_2n_plus1  = fib(2 * n + 1);
    seq_t L2n = f_2n_minus1 + f_2n_plus1;

    return Ln * L2n;
}
