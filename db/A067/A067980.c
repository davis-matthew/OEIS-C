/*
 * A067980  Convolution of L(n+1) := A000204(n+1) (Lucas), n>=0, with L(n+2),
 *          n>=0.
 * https://oeis.org/A067980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+3)*F(n+1)+3*(n+1)*F(n), where F is Fibonacci
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast doubling: returns F(n) in *fn, F(n+1) in *fn1 */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);                 /* a = F(k), b = F(k+1) , k = n/2 */

    /* Compute F(2k) and F(2k+1) using the doubling identities */
    c = a * ( (b << 1) - a );                 /* F(2k)   = F(k) * (2*F(k+1) - F(k)) */
    d = a * a + b * b;                        /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if ((n & 1) == 0) {                       /* n is even  */
        *fn  = c;
        *fn1 = d;
    } else {                                  /* n is odd   */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);                  /* fn  = F(n),  fn1 = F(n+1) */

    seq_t n_seq = (seq_t)n;
    seq_t term1 = ( ((seq_t)4 * n_seq) + 3 ) * fn1;          /* (4n+3)*F(n+1) */
    seq_t term2 = ( (n_seq + 1) * (seq_t)3 ) * fn;           /* 3*(n+1)*F(n) */

    return term1 + term2;
}
