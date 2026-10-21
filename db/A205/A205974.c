/*
 * A205974  a(n) = Fibonacci(n)*A033719(n) for n>=1, with a(0)=1, where
 *          A033719 lists the coefficients in theta_3(q)*theta_3(q^7).
 * https://oeis.org/A205974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 48, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A205974
 * Formula: a(n) = Fibonacci(n) * A033719(n)   (n ≥ 1),
 *          a(0) = 1
 *
 * A033719(n) is the number of representations of n as
 *   m^2 + 7*n^2   (m,n ∈ ℤ)
 * counted with sign multiplicity: each pair with m≠0,n≠0 gives 4,
 * each pair with one of them zero gives 2.
 */

#include <stdbool.h>
#include <stddef.h>

/* ---------- integer square root -------------------------------------- */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { low = mid + 1; continue; }
        if (mid <= n / mid) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* ---------- (optional) fast Fibonacci --------------------------------- */
static inline void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) { *fn = 0; *fn1 = 1; return; }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);
    seq_t t = b + b;
    seq_t c = a * (t - a);
    seq_t d = a * a + b * b;
    if ((n & 1) == 0) { *fn = c; *fn1 = d; }
    else { *fn = d; *fn1 = c + d; }
}
static inline seq_t fib(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);
    return fn;
}

/* ---------- A033719 coefficient --------------------------------------- */
static inline seq_t A033719(idx_t k)
{
    if (k == 0) return 1;              /* only (0,0) */
    seq_t pos = 0;                     /* m>0 , n>0 */
    idx_t max_n = isqrt_u64(k / 7);
    for (idx_t n = 1; n <= max_n; ++n) {
        idx_t rem = k - 7 * n * n;
        if (rem == 0) continue;         /* m = 0 case handled below */
        idx_t m = isqrt_u64(rem);
        if (m * m == rem) pos++;        /* m > 0 automatically */
    }
    seq_t result = pos * 4;

    /* m = 0, n > 0 */
    if (k % 7 == 0) {
        idx_t t = k / 7;
        idx_t r = isqrt_u64(t);
        if (r * r == t) result += 2;
    }
    /* n = 0, m > 0 */
    idx_t r = isqrt_u64(k);
    if (r * r == k) result += 2;

    return result;
}

/* ---------- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* definition */
    seq_t fn = fib(n);                  /* Fibonacci(n) */
    seq_t coeff = A033719(n);           /* A033719(n) */
    return fn * coeff;
}
