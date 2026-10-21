/*
 * A209434  Table T(n,m), read by antidiagonals, is the number of subsets of
 *          {1,...,n} which do not contain two elements whose difference is
 *          m+1.
 * https://oeis.org/A209434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 275, u32 n <= 1080, u64 n <= 4277,
 *            u128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* OEIS A209434:  a(n) = T(k-j, j) where 
 * k = ⌊(√(8n+1)-1)/2⌋  and  j = n - k(k+1)/2.
 * The table entry T(p,q) is defined by
 *  T(p,q) = ∏_{i=0}^{q} F(⌊(p+i)/(q+1)⌋ + 2),
 * where F(m) is the m‑th Fibonacci number (F(0)=0, F(1)=1). */

static seq_t fib_small(idx_t n)
{
    seq_t a = 0;
    seq_t b = 1;
    if (n == 0) return a;
    if (n == 1) return b;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    for (idx_t bit = (idx_t)1ULL << 62; bit > 0; bit >>= 1) {
        idx_t trial = r + bit;
        if (trial <= x / trial) {
            r = trial;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    /* find k such that k*(k+1)/2 ≤ n < (k+1)*(k+2)/2 */
    idx_t t = n * 8 + 1;
    idx_t r = isqrt_u64(t);
    idx_t k = (r - 1) / 2;

    idx_t c_prev = (k * (k + 1)) / 2;
    idx_t j = n - c_prev;          /* 0 ≤ j ≤ k */
    idx_t m = j;
    idx_t n1 = k - j;              /* first index on the diagonal */

    seq_t prod = 1;
    for (idx_t i = 0; i <= m; ++i) {
        idx_t g = ((n1 + i) / (m + 1)) + 2;
        prod *= fib_small(g);
    }

    return prod;
}
