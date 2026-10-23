/*
 * A122525  Triangle read by rows: G(s, rho) = ((s-1)!/s)*Sum_{i=0..s-1}
 *          ((s-i)/i!)*(s*rho)^i.
 * https://oeis.org/A122525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 62, u64 n <= 160,
 *            u128 n <= 419, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A122525
   Triangle read by rows: G(s,ρ) = ((s‑1)!/s) * Σ_{i=0}^{s-1} ((s-i)/i!)*(sρ)^i.
   The flattened sequence a(n) lists the coefficients of ρ⁰…ρ^{s‑1} for each
   successive integer s (starting at s = 1).  The index n (≥ 1) is mapped to
   the row s where s(s+1)/2 ≥ n, and the column k = n – s(s‑1)/2 – 1.
   The entry equals T(s‑1,k) where T(0,0)=0! = 1 and
   T(n,k+1)=T(n,k) * ((n+1)*(n-k)) / ((k+1)*(n-k+1)).
*/

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#  include <type_traits>
#endif

/* Compute m! as a seq_t. Works for all seq_t types. */
static seq_t factorial_uint64(idx_t m)
{
    seq_t f = (seq_t)1;
    for (idx_t i = 1; i <= m; ++i)
        f = f * (seq_t)i;
    return f;
}

/* Multiplication followed by exact division.
   For integral seq_t types we use 128‑bit arithmetic when available.
   For non‑integral (big‑int) types we rely on seq_t's own operators. */
#ifdef __cplusplus
static inline seq_t mul_div(seq_t t, seq_t num, seq_t den)
{
    if constexpr (std::is_integral<seq_t>::value) {
        return ((unsigned __int128)t * num) / den;
    } else {
        return (t * num) / den;
    }
}
#else
static inline seq_t mul_div(seq_t t, seq_t num, seq_t den)
{
    return ((__int128)t * num) / den;
}
#endif

/* Return the n‑th term of A122525 (n ≥ 1). */
seq_t a(idx_t n)
{
    /* 1. Locate the triangle row s such that s*(s+1)/2 ≥ n. */
    idx_t low  = 1;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 cum = (__int128)mid * (mid + 1) / 2;
        if (cum >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t s = low;                         /* row number (1‑based) */

    /* 2. Compute 0‑based column index k. */
    idx_t prev_sum = (s - 1) * s / 2;      /* terms before this row */
    idx_t k = n - prev_sum - 1;            /* column index within the row */

    /* 3. Set n' = s-1 for the recurrence (zero‑based row). */
    idx_t np = s - 1;

    /* 4. Start from T(n',0) = (n')! . */
    seq_t t = factorial_uint64(np);

    /* 5. Use the recurrence to advance to column k. */
    for (idx_t i = 0; i < k; ++i) {
        seq_t num = (seq_t)(np + 1) * (seq_t)(np - i);
        seq_t den = (seq_t)(i + 1)     * (seq_t)(np - i + 1);
        t = mul_div(t, num, den);
    }

    return t;
}
