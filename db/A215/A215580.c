/*
 * A215580  Partial sums of A215602.
 * https://oeis.org/A215580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = L(4n+2) - 1 a(2n+1) = L(4n+4) - 2 where L(k) is the k‑th
 *            Lucas number (L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2))
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute F(n) and F(n+1) via fast doubling (iterative). */
/* seq_t holds the Fibonacci numbers; idx_t is the index type (uint64_t). */
static inline void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;   /* F(0) */
    seq_t b = 1;   /* F(1) */

    for (int i = 63; i >= 0; --i) {
        /* d = a * (2*b - a) */
        seq_t two_b = b + b;
        seq_t d = a * (two_b - a);

        /* e = a*a + b*b */
        seq_t e = a * a + b * b;

        if ((n >> i) & 1) {
            a = e;
            b = d + e;
        } else {
            a = d;
            b = e;
        }
    }
    *fn  = a;  /* F(n)   */
    *fn1 = b;  /* F(n+1) */
}

static inline seq_t lucas_at(idx_t k)
{
    seq_t f, fp1;
    fib_pair(k, &f, &fp1);          /* f  = F(k),   fp1 = F(k+1) */
    /* Lucas: L(k) = 2*F(k+1) - F(k) */
    seq_t two_fp1 = fp1 + fp1;
    return two_fp1 - f;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                 /* even index: n = 2*m */
        idx_t m = n >> 1;               /* m = n/2 */
        idx_t k = 4 * m + 2;            /* Lucas index for even n */
        seq_t L = lucas_at(k);
        return L - 1;
    } else {                            /* odd index:  n = 2*m + 1 */
        idx_t m = n >> 1;               /* m = (n-1)/2 */
        idx_t k = 4 * m + 4;            /* Lucas index for odd n */
        seq_t L = lucas_at(k);
        return L - 2;
    }
}
