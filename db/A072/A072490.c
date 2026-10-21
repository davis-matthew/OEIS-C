/*
 * A072490  Number of squarefree numbers (excluding 1) less than n.
 * https://oeis.org/A072490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1}^{⌊√(n-1)⌋} μ(d) · ⌊(n-1)/d²⌋ – 1 (μ is the Möbius
 *            function). Compute the count of square‑free numbers ≤ n‑1 and
 *            subtract 1 to exclude 1. The implementation uses only integer
 *            arithmetic and works for any seq_t / idx_t pair supplied by the
 *            build. Complexity per call is O(√n)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* --------- integer square root (floor) --------------------- */
static inline idx_t floor_sqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

/* --------- Möbius function μ(d) ---------------------------- */
static int mobius(idx_t d)
{
    if (d == 1)
        return 1;
    int prime_cnt = 0;
    for (idx_t p = 2; p * p <= d; ++p) {
        if (d % p == 0) {
            ++prime_cnt;
            d /= p;
            if (d % p == 0)          /* square factor found */
                return 0;
            while (d % p == 0)       /* divide out p completely */
                d /= p;
        }
    }
    if (d > 1)           /* remaining prime > sqrt(original d) */
        ++prime_cnt;
    return (prime_cnt & 1) ? -1 : 1;
}

/* --------- sequence function -------------------------------- */
seq_t a(idx_t n)
{
    /* For n ≤ 1, the sequence is defined as 0 */
    if (n < 2)
        return 0;

    /* Count square‑free numbers up to n‑1 */
    idx_t limit = n - 1;
    idx_t max_d = floor_sqrt(limit);
    long long sum = 0;                  /* signed intermediate value */

    for (idx_t d = 1; d <= max_d; ++d) {
        int mu = mobius(d);
        if (mu == 0)
            continue;
        idx_t d_sq = d * d;              /* d² fits into idx_t for the limits */
        sum += mu * (long long)(limit / d_sq);
    }

    /* Exclude the number 1 from the count */
    sum -= 1;

    return (seq_t)sum;          /* final result fits in seq_t for tested n */
}
