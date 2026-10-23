/*
 * A100389  a(n) = Bell(n) - Fibonacci(n).
 * https://oeis.org/A100389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n) - Fibonacci(n)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

typedef struct { seq_t a; seq_t b; } FibPair;

/* Fast‑doubling Fibonacci: returns {F(n), F(n+1)} */
static FibPair fib_pair(idx_t n)
{
    if (n == 0) return (FibPair){ (seq_t)0, (seq_t)1 };
    FibPair p = fib_pair(n >> 1);
    seq_t a = p.a;
    seq_t b = p.b;
    seq_t two_b = (seq_t)2 * b;
    seq_t c = a * (two_b - a);          /* F(2k)   */
    seq_t d = a * a + b * b;            /* F(2k+1) */
    if ((n & 1) == 0)
        return (FibPair){ c, d };
    else
        return (FibPair){ d, c + d };
}

seq_t a(idx_t n)
{
    /* Compute Bell(n) via recurrence B_{m+1} = Σ_{k=0}^{m} C(m,k) B_k */
    seq_t *B = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!B) return (seq_t)0;          /* allocation failure → 0 */
    B[0] = (seq_t)1;
    for (idx_t m = 0; m < n; ++m) {
        seq_t sum  = (seq_t)0;
        seq_t binom = (seq_t)1;                 /* C(m,0) */
        for (idx_t k = 0; k <= m; ++k) {
            if (k > 0)
                binom = binom * ( (seq_t)(m - k + 1) ) / (seq_t)k;
            sum += binom * B[k];
        }
        B[m + 1] = sum;
    }
    seq_t bell_n = B[n];
    free(B);

    /* Compute Fibonacci(n) */
    seq_t fib_n = fib_pair(n).a;

    /* Result: Bell(n) - Fibonacci(n) (non‑negative for all n≥0) */
    return bell_n - fib_n;
}
