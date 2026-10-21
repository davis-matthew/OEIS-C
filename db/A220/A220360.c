/*
 * A220360  a(n) = Fibonacci(n-1) * Fibonacci(n+1) * Fibonacci(n+2).
 * https://oeis.org/A220360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n-1) * Fibonacci(n+1) * Fibonacci(n+2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Fibonacci numbers with fast doubling (O(log n)) and use the identities
   F(n-1) = F(n+1) - F(n) and F(n+2) = F(n+1) + F(n).  */

static void fib_pair_internal(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;          /* F(0)   */
    seq_t b = 1;          /* F(1)   */

    if (n == 0) {
        *fn  = a;
        *fn1 = b;
        return;
    }

    /* find position of most significant bit */
    int msb = 0;
    idx_t tmp = n;
    while ((tmp >>= 1) != 0)
        msb++;

    for (int i = msb; i >= 0; --i) {
        /* doubling formulas:
           c = F(2k)   = F(k) * (2*F(k+1) - F(k))
           d = F(2k+1) = F(k)^2 + F(k+1)^2
        */
        seq_t c = a * (((b << 1) - a));
        seq_t d = a * a + b * b;
        a = c;
        b = d;
        if ((n >> i) & 1) {     /* odd step: shift to k+1 */
            seq_t a_next = d;
            seq_t b_next = c + d;
            a = a_next;
            b = b_next;
        }
    }
    *fn  = a;
    *fn1 = b;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    seq_t f_n, f_np1;

    fib_pair_internal(n, &f_n, &f_np1);   /* F(n), F(n+1) */

    seq_t f_nm1 = f_np1 - f_n;            /* F(n-1) = F(n+1) - F(n) */
    seq_t f_np2 = f_np1 + f_n;            /* F(n+2) = F(n+1) + F(n) */

    return f_nm1 * f_np1 * f_np2;
}
