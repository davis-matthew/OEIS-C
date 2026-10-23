/*
 * A134498  a(n) = Fibonacci(7n).
 * https://oeis.org/A134498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(7*n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes any Fibonacci number by fast doubling; works for all
 * supported seq_t types. */

seq_t a(idx_t n)
{
    /* compute the index for Fibonacci */
    idx_t k = n * 7u;      /* 7*n */

    /* base case: F(0) = 0 */
    if (k == 0)
        return (seq_t)0;

    /* fast doubling variables: a = F(current), b = F(current+1) */
    seq_t a = 0;            /* F(0)  */
    seq_t b = 1;            /* F(1)  */

    /* find most significant set bit in k */
    int high = 63;
    while (high > 0 && !(k & ((idx_t)1 << high)))
        high--;

    /* iterate from most significant bit to least */
    for (int i = high; i >= 0; --i) {
        /* doubling formulas:
         *   c = F(2n)   = F(n) * (2*F(n+1) - F(n))
         *   d = F(2n+1) = F(n)^2 + F(n+1)^2
         */
        seq_t t = (b << 1) - a;   /* 2*b - a */
        seq_t c = a * t;          /* F(2n)   */
        seq_t d = a * a + b * b;  /* F(2n+1) */

        /* if current bit of k is set, advance to n' = 2n+1 */
        if (k & ((idx_t)1 << i)) {
            a = d;
            b = c + d;
        } else {                  /* advance to n' = 2n   */
            a = c;
            b = d;
        }
    }

    return a;   /* F(k) = Fibonacci(7*n) */
}
