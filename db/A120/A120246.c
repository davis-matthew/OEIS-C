/*
 * A120246  a(1) = 1. a(m(m+1)/2 + k) = m + a(k), 1 <= k <= m+1, m >= 1.
 * https://oeis.org/A120246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; for n = m(m+1)/2 + k (1 ≤ k ≤ m+1) a(n) = m + a(k)
 * Range:     u8 n <= 10011, u16 n <= 10011, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 95 terms through n = 9175 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers needed for types and sqrtl. */
#include <stdint.h>
#include <math.h>

/* Helper: find the largest m such that m*(m+1)/2 < n,  n ≥ 2. */
static inline idx_t triangular_upper_idx(idx_t n)
{
    /* n = 1 is never here; guard for tiny values. */
    if (n <= 2) return 1;

    /* Use long double sqrt for a good initial approximation. */
    long double d = sqrtl((long double)(n - 1) * 8.0L + 1.0L);
    idx_t m = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct any possible error from floating point conversion. */
    unsigned __int128 tri = (unsigned __int128)m * (m + 1) / 2;
    while (tri >= n) {
        --m;
        tri = (unsigned __int128)m * (m + 1) / 2;
    }
    while ((unsigned __int128)(m + 1) * (m + 2) / 2 < n) {
        ++m;
    }
    return m;
}

/* Main function implementing the recurrence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* not defined for 0, safe fallback */

    seq_t result = 1;                  /* base: a(1) = 1 */
    idx_t cur = n;

    while (cur > 1) {
        idx_t m = triangular_upper_idx(cur);
        idx_t k = cur - (idx_t)((unsigned __int128)m * (m + 1) / 2);
        result += (seq_t)m;
        cur = k;
    }
    return result;
}
