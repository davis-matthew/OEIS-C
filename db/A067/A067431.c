/*
 * A067431  Convolution of Fibonacci F(n+1), n>=0, with F(n+8), n>=0.
 * https://oeis.org/A067431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((76*n + 5*21)*F(n+1) + 47*(n+1)*F(n))/5
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute Fibonacci numbers F(n) and F(n+1) iteratively. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = (seq_t)0;          /* F(0) */
    seq_t b = (seq_t)1;          /* F(1) */

    /* Iterate up to n+1 to obtain F(n) and F(n+1). */
    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t c = a + b;
        a    = b;
        b    = c;
    }

    *fn  = a;   /* F(n)   */
    *fn1 = b;   /* F(n+1) */
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);

    /* Use casts to seq_t as required for intermediate computations. */
    seq_t temp1 = (seq_t)76 * (seq_t)n + (seq_t)105;              /* 5*21 = 105 */
    seq_t temp2 = (seq_t)47 * (seq_t)(n + 1);

    seq_t numerator = temp1 * fn1 + temp2 * fn;

    /* Exact division by 5. */
    return numerator / (seq_t)5;
}
