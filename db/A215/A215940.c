/*
 * A215940  Difference between the n-th and the first (identity) permutation
 *          of (0,...,m-1), interpreted as a decimal number, divided by 9 (for
 *          any m for which 10! >= m! >= n).
 * https://oeis.org/A215940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   * a(n) = (P(n) – P(1)) / 9 * where P(n) is the decimal value of
 *            the n‑th lexicographic permutation * of the digits
 *            (0,1,...,m‑1), m being the minimal integer with m! ≥ n. * P(1)
 *            is the identity permutation 0,1,2,…,m‑1
 * Range:     u8 n <= 6, u16 n <= 150, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const idx_t MAX_M = 25;   /* enough for n up to 10^7 (20! > 2·10^18) */

seq_t a(idx_t n)
{
    /* ---------- find minimal m such that m! >= n ---------- */
    idx_t m = 1;
    idx_t fact = 1;
    while (fact < n) {
        ++m;
        fact *= m;
    }

    /* ---------- factorial table up to m ---------- */
    idx_t fact_tbl[MAX_M];
    fact_tbl[0] = (idx_t)1;
    for (idx_t i = 1; i <= m; ++i)
        fact_tbl[i] = fact_tbl[i-1] * i;

    /* ---------- build n‑th permutation (0‑based rank = n‑1) ---------- */
    idx_t rank = n - 1;
    idx_t remaining[MAX_M];
    for (idx_t i = 0; i < m; ++i)
        remaining[i] = i;

    seq_t perm_val = 0;                /* P(n) */
    idx_t rem_size = m;
    for (idx_t pos = 0; pos < m; ++pos) {
        idx_t f = fact_tbl[m - 1 - pos];
        idx_t idx = rank / f;
        rank %= f;

        /* digit at current position */
        int digit = remaining[idx];
        perm_val = perm_val * (seq_t)10 + (seq_t)digit;

        /* delete used digit from remaining list */
        for (idx_t j = idx; j + 1 < rem_size; ++j)
            remaining[j] = remaining[j + 1];
        --rem_size;
    }

    /* ---------- compute P(1) (identity permutation) ---------- */
    seq_t identity_val = 0;
    for (idx_t d = 0; d < m; ++d)
        identity_val = identity_val * (seq_t)10 + (seq_t)d;

    /* ---------- result = (P(n)-P(1))/9 ---------- */
    seq_t diff = perm_val - identity_val;
    return diff / (seq_t)9;
}
