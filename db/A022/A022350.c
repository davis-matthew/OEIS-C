/*
 * A022350  Fibonacci sequence beginning 0, 16.
 * https://oeis.org/A022350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16 * A000045(n) = 16 * F(n)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 978 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fast_fib(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;      /* F(0) */
        *fn1 = 1;      /* F(1) */
        return;
    }

    seq_t a, b;
    fast_fib(n >> 1, &a, &b);  /* a = F(k), b = F(k+1) where k = n/2 */

    /* Fast doubling identities:
       F(2k)   = F(k) * (2*F(k+1) - F(k))
       F(2k+1) = F(k)^2 + F(k+1)^2
    */
    seq_t t1 = a * ((b << 1) - a);      /* F(2k)   */
    seq_t t2 = a * a + b * b;           /* F(2k+1) */

    if ((n & 1) == 0) {
        *fn  = t1;          /* F(n)   */
        *fn1 = t2;          /* F(n+1) */
    } else {
        *fn  = t2;          /* F(n)   */
        *fn1 = t1 + t2;     /* F(n+1) */
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fast_fib(n, &fn, &fn1);
    fn *= (seq_t)16;          /* a(n) = 16 * F(n) */
    return fn;
}
