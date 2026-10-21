/*
 * A094440  Triangular array read by rows: T(n,k) = Fibonacci(n+1-k)*C(n,k-1),
 *          k = 1..n; n >= 1.
 * https://oeis.org/A094440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 96, u32 n <= 334, u64 n <= 1240,
 *            u128 n <= 4593, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) reads the triangular array T(n,k)=Fib(n+1-k)*C(n,k-1) by rows.
 * For a given linear index n (1‑based) we first find the row r such that
 * r*(r+1)/2 ≥ n, and let k = n - r*(r-1)/2.  Then
 * a(n) = Fib(r+1-k) * C(r, k-1).
 *
 * Complexity: O(√n).  The helper functions fib_seq and binom_idx compute
 * the Fibonacci number and binomial coefficient multiplicatively, so there
 * is no need for external libraries.
 */

#include <stdint.h>   /* ensures uint64_t etc.  */

static idx_t row_of_idx(idx_t pos)
{
    /* binary search for the smallest r with r*(r+1)/2 >= pos */
    idx_t low = 1, high = pos;  /* r never exceeds pos */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= pos)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Compute Fibonacci(F) for F >= 1 (F=1 => 1, F=2 => 1, etc.)  */
static seq_t fib_seq(idx_t F)
{
    if (F <= 1)  /* F == 1 for our usage */
        return (seq_t)1;

    seq_t a = (seq_t)0;
    seq_t b = (seq_t)1;
    for (idx_t i = 2; i <= F; ++i) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

/* Compute binomial coefficient C(n,k) for 0 <= k <= n */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k == 0 || k == n)
        return (seq_t)1;

    /* Use symmetry to reduce the loop length */
    if (k > n - k)
        k = n - k;

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = n - k + i;          /* numerator term */
        res = (res * (seq_t)num) / (seq_t)i;  /* exact division */
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n=0; return 0 as a safe default */
        return (seq_t)0;

    /* Locate row r and column k (1‑based) */
    idx_t r  = row_of_idx(n);
    idx_t prev = (r - 1) * r / 2;        /* terms before row r */
    idx_t k   = n - prev;                /* column within the row */

    /* Indices for Fibonacci and binomial */
    idx_t fib_idx   = r + 1 - k;         /* ≥ 1 */
    idx_t binom_k   = k - 1;             /* may be 0 */

    seq_t fib_val   = fib_seq(fib_idx);
    seq_t binom_val = binom_idx(r, binom_k);

    return fib_val * binom_val;
}
