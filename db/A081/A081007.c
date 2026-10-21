/*
 * A081007  a(n) = Fibonacci(4n+1) - 1, or Fibonacci(2n)*Lucas(2n+1).
 * https://oeis.org/A081007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4n+1) - 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the fast‑doubling method to compute
   Fibonacci numbers efficiently.  The function fib_pair returns
   F(k) and F(k+1). */

static void fib_pair(idx_t k, seq_t *Fk, seq_t *Fk1)
{
    if (k == 0) {
        *Fk  = (seq_t)0;
        *Fk1 = (seq_t)1;
        return;
    }
    seq_t a, b, c, d;
    fib_pair(k >> 1, &a, &b);          /* a = F(m), b = F(m+1), m = k/2 */

    /* c = F(2m)   = a * ( 2*b - a )            */
    seq_t two_b = b + b;
    seq_t two_b_minus_a = two_b - a;
    c = a * two_b_minus_a;               /* F(2m)    */

    /* d = F(2m+1) = a*a + b*b               */
    d = (a * a) + (b * b);               /* F(2m+1)  */

    if ((k & 1) == 0) {                  /* k even   */
        *Fk  = c;
        *Fk1 = d;
    } else {                              /* k odd    */
        *Fk  = d;
        *Fk1 = c + d;
    }
}

seq_t a(idx_t n)
{
    idx_t m = 4 * n + 1;                  /* argument to Fibonacci   */
    seq_t fn, fn1;
    fib_pair(m, &fn, &fn1);               /* fn = F(4n+1)            */
    return fn - (seq_t)1;                 /* a(n) = F(4n+1) - 1     */
}
