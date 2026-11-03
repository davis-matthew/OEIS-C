/*
 * A062822  Sum of divisors of the squarefree numbers: sigma(A005117(n)).
 * https://oeis.org/A062822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ( A005117(n) ) = σ( the n-th squarefree number )
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 979 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Helper: compute σ(n) = ∏_{p|n} (p+1) over distinct prime factors p */
static seq_t sigma_of_squarefree(idx_t n)
{
    seq_t res = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            res *= (seq_t)(p + 1);
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) res *= (seq_t)(m + 1);
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used but safe */

    /* Cache for already computed terms */
    static seq_t cache[10001];     /* enough for n <= 10000 */
    static size_t cached = 0;      /* number of terms cached (>=1) */
    static idx_t next_candidate = 1;  /* candidate to test for squarefreeness */

    /* Extend cache up to requested n */
    while (cached < n) {
        /* Find next squarefree integer */
        while (1) {
            idx_t cand = next_candidate;
            ++next_candidate;
            /* Check if cand has any squared prime factor */
            idx_t temp = cand;
            idx_t p = 2;
            int is_sqfree = 1;
            while (p * p <= temp) {
                if (temp % (p * p) == 0) { is_sqfree = 0; break; }
                ++p;
            }
            if (is_sqfree) {
                cache[cached] = sigma_of_squarefree(cand);
                ++cached;
                break;
            }
        }
    }

    return cache[n - 1];
}
