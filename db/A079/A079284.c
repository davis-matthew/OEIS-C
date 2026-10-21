/*
 * A079284  Diagonal sums of triangle A008949.
 * https://oeis.org/A079284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n+3) - 2^⌊(n+1)/2⌋
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* fast doubling Fibonacci: returns (F_n, F_{n+1}) in fn and fn1 */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a=F_k, b=F_{k+1} where k = n/2 */
    seq_t two_b = b << 1;              /* 2*F_{k+1} */
    seq_t c = a * (two_b - a);         /* F_{2k} = F_k * (2*F_{k+1} - F_k) */
    seq_t d = a * a + b * b;           /* F_{2k+1} = F_k^2 + F_{k+1}^2 */
    if ((n & 1) == 0) {
        *fn = c;
        *fn1 = d;
    } else {
        *fn = d;
        *fn1 = c + d;                  /* F_{2k+2} = F_{2k} + F_{2k+1} */
    }
}

/* compute 2^exp  (exp is idx_t) */
static seq_t pow2(idx_t exp)
{
    seq_t res = (seq_t)1;
    return res << exp;
}

seq_t a(idx_t n)
{
    /* compute Fibonacci(n+3) */
    seq_t fn, fn1;
    fib_pair(n + 3, &fn, &fn1);   /* fn = F_{n+3} */

    /* compute 2^floor((n+1)/2) */
    idx_t shift = (n + 1) / 2;
    seq_t p2 = pow2(shift);

    return fn - p2;
}
