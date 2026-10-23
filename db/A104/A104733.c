/*
 * A104733  Triangle T(n,k) = sum_{j=k..n} Fibonacci(n-j+1)*Fibonacci(k+1),
 *          read by rows, 0<=k<=n.
 * https://oeis.org/A104733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(c+1) * ( Fibonacci(r-c+3) - 1 ) * where r =
 *            floor( (sqrt(8*n+1)-1)/2 ), c = n - r*(r+1)/2 * (row r, column c
 *            of the lower‑triangular array T(n,k) defined in OEIS A104733)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

/* Return F(k) where F(0)=0, F(1)=1, using only seq_t arithmetic. */
static seq_t fib_seq(idx_t k)
{
    if (k == 0) return (seq_t)0;
    if (k == 1) return (seq_t)1;
    seq_t a = 0, b = 1, c;
    for (idx_t i = 2; i <= k; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

/* compute a(n) for the sequence A104733 */
seq_t a(idx_t n)
{
    /* 1. find the row r */
    long double m = 8.0L * (long double)n + 1.0L;
    long double d = sqrtl(m);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);
    /* correct for rounding errors */
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* 2. compute column c */
    idx_t c = n - r * (r + 1) / 2;

    /* 3. evaluate the formula */
    seq_t f1 = fib_seq(c + 1);              /* Fibonacci(c+1)      */
    seq_t f2 = fib_seq(r - c + 3) - (seq_t)1; /* Fibonacci(r-c+3)-1*/
    return f1 * f2;
}
