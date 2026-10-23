/*
 * A140835  A triangular sequence from a vector a(n) times a triangular tensor
 *          t(n,m): T(n,m)=a(n).t(n,m); a(n)=Fibonacci(n);A000045(n):
 *          t(n,m)=Binomial(n,GCD(n,m)).
 * https://oeis.org/A140835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = Fib(r) * Binomial(r, gcd(r, m)) where r =
 *            floor((√(8n-1)-1)/2) is the 0‑based row index, start = r(r+1)/2
 *            is the 0‑based index of the first element in row r, and m = n -
 *            start - 1 is the 0‑based column index within that row
 * Range:     u8 n <= 38, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"


static inline seq_t fib_n(idx_t n)
{
    seq_t a = 0, b = 1;
    if (n == 0) return a;
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* find 0‑based row r such that (r+1)(r+2)/2 ≥ n */
    idx_t r = 0;
    while (((__int128)(r + 1) * (r + 2)) / 2 < n)
        ++r;

    idx_t start = r * (r + 1) / 2;     /* 0‑based start index of row r */
    idx_t m = n - start - 1;           /* 0‑based column index within the row */

    seq_t fib_val = fib_n(r);          /* Fib(r) for this row (Fib(0)=0), matches a(n) */
    idx_t g = gcd_idx(r, m);
    seq_t binom_val = binomial(r, g);

    return fib_val * binom_val;
}
