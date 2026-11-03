/*
 * A228633  Number of s in {1,...,n}^n having shortest run with the same value
 *          of length 7.
 * https://oeis.org/A228633
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 38, u64 n <= 66, u128 n <= 118,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 172 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228633: Number of s in {1,…,n}^n having shortest run with the same value of length 7.
   Formula implemented:
      a(n) = Σ_{r=1}^{⌊n/7⌋} n·(n‑1)^(r‑1)·[C(n‑6r‑1,r‑1) – C(n‑7r‑1,r‑1)],
   where C(x,y) is the binomial coefficient and C(x,y)=0 if x<y or x<0. */
/* Note: C(x,0)=1 for x≥0, and C(x,0)=0 for x<0. */

#include <stdint.h>

/* Helper: compute binomial coefficient C(n,k) for n,k≥0.
   Uses multiplicative formula and returns 0 if n<k or n<0. */
static seq_t binom_safe(int64_t n_signed, idx_t k) {
    if (n_signed < 0) return 0;
    idx_t n = (idx_t)n_signed;
    if (k > n) return 0;
    if (k == 0) return 1;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - i + 1) / i; division is exact at each step. */
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n < 7) return 0;          /* behavior below offset is irrelevant */

    seq_t result = 0;
    seq_t pow_term = 1;           /* (n-1)^(r-1) starts with r=1 */

    /* r ranges from 1 to floor(n/7) */
    for (idx_t r = 1; r <= n / 7; ++r) {
        int64_t val1 = (int64_t)n - 6 * (int64_t)r - 1;   /* n-6r-1 */
        int64_t val2 = (int64_t)n - 7 * (int64_t)r - 1;   /* n-7r-1 */
        idx_t k = r - 1;

        seq_t bin1 = binom_safe(val1, k);
        seq_t bin2 = binom_safe(val2, k);
        seq_t diff  = bin1 - bin2;          /* >=0 by construction */

        if (diff) {
            seq_t term = (seq_t)n * pow_term * diff;
            result += term;
        }

        /* Prepare pow_term for next r: multiply by (n-1) */
        pow_term *= (seq_t)(n - 1);
    }

    return result;
}
