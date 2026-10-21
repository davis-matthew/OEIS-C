/*
 * A214221  (n^3)-th Lucas number (beginning at 2).
 * https://oeis.org/A214221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas( n^3 ) (Lucas numbers with L(0)=2, L(1)=1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t* fn, seq_t* fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);      /* a = F(k), b = F(k+1)   k = n/2 */
    seq_t two_b_minus_a = (b << 1) - a;   /* 2*F(k+1) - F(k)       */
    c = a * two_b_minus_a;           /* F(2k)                 */
    d = a * a + b * b;               /* F(2k+1)               */
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
    /* Special cases for small n where the Fibonacci proxy is awkward */
    if (n == 0) return 2;            /* L(0) */
    if (n == 1) return 1;            /* L(1) */

    /* Compute exponent n^3 (fits in 64‑bit for the supported ranges) */
    idx_t exp = n * n * n;

    seq_t f_k_minus1, f_k;
    fib_pair(exp - 1, &f_k_minus1, &f_k);   /* f_k_minus1 = F(exp-1), f_k = F(exp) */

    /* Lucas number: L(exp) = F(exp) + 2*F(exp-1) */
    seq_t lucas = f_k + (f_k_minus1 << 1);
    return lucas;
}
