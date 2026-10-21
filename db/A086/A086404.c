/*
 * A086404  Square array of numbers T(n,k) =
 *          ((1+sqrt(3))*(k+sqrt(3))^n-(1-sqrt(3))*(k-sqrt(3))^n)/(2*sqrt(3)),
 *          read by antidiagonals.
 * https://oeis.org/A086404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086404 – square array read by antidiagonals.
   The array T(n,k) is defined by
     T(0,k) = 1,
     T(1,k) = k + 1,
     T(n+2,k) = 2*k*T(n+1,k) - (k*k - 3)*T(n,k)  (n ≥ 0),
   i.e. T(n,k) = ((1+√3)(k+√3)ⁿ – (1–√3)(k–√3)ⁿ)/(2√3).
   The sequence a(n) is obtained by reading T(n,k) along antidiagonals:
   a(0)=T(0,0), a(1)=T(0,1), a(2)=T(1,0), a(3)=T(0,2), …
   This implementation computes a(n) in O(√n) time using integer arithmetic only. */
#include <math.h>
#include <stdint.h>

/* integer square root: floor(sqrt(x)) for non‑negative x */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = (uint64_t)sqrt((long double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* compute T(m,k) using the recurrence */
static seq_t T_value(idx_t m, idx_t k)
{
    if (m == 0)
        return (seq_t)1;
    seq_t t0 = (seq_t)1;
    seq_t t1 = (seq_t)k + 1;
    if (m == 1)
        return t1;

    seq_t k_sq = (seq_t)k * (seq_t)k;
    seq_t mult1 = 2 * (seq_t)k;          /* 2*k */
    seq_t mult2 = k_sq - 3;              /* k*k - 3 */

    seq_t prev2 = t0, prev1 = t1, curr = 0;
    for (idx_t i = 2; i <= m; ++i) {
        curr = mult1 * prev1 - mult2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}

/* Main function */
seq_t a(idx_t n)
{
    /* find diagonal d: smallest d with (d+1)(d+2)/2 > n */
    uint64_t d = (isqrt_u64(8 * n + 1) - 1) / 2;

    uint64_t prev_terms = d * (d + 1) / 2;   /* terms before diagonal d */
    uint64_t offset = n - prev_terms;        /* 0‑based position inside diagonal */

    idx_t n_row = (idx_t)offset;           /* row index in T */
    idx_t k     = (idx_t)(d - offset);     /* column index in T */

    return T_value(n_row, k);
}
