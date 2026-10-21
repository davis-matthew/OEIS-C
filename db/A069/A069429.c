/*
 * A069429  Half the number of 3 X n binary arrays with no path of adjacent
 *          1's or adjacent 0's from top row to bottom row.
 * https://oeis.org/A069429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) * Fibonacci(2*n+2) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, used to define idx_t in builds */

/* Helper structure for fast‑doubling Fibonacci */
typedef struct {
    seq_t f;
    seq_t g;   /* represents F(k) and F(k+1) */
} FibPair;

/* Fast doubling: returns (F(k), F(k+1)) */
static FibPair fib_pair(idx_t k) {
    if (k == 0) {
        FibPair r = {0, 1};
        return r;
    }
    FibPair p = fib_pair(k >> 1);
    seq_t a = p.f; /* F(m) */
    seq_t b = p.g; /* F(m+1) */

    /* c = F(2m) = F(m) * [2*F(m+1) – F(m)] */
    seq_t c = a * ((b + b) - a);

    /* d = F(2m+1) = F(m)^2 + F(m+1)^2 */
    seq_t d = a * a + b * b;

    if ((k & 1) == 0) {
        FibPair r = {c, d};
        return r;
    } else {
        FibPair r = {d, c + d};
        return r;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* n is 1‑based, n ≥ 1 */
    idx_t shift = n - 1;                 /* exponent for 2 */
    FibPair fp = fib_pair(2 * n + 2);    /* (F(2n+2), F(2n+3)) */
    seq_t fib_term = fp.f;               /* Fib(2n+2) */
    seq_t pow2 = ((seq_t)1) << shift;    /* 2^(n-1) */
    return pow2 * fib_term;
}
