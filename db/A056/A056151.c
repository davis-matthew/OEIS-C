/*
 * A056151  Distribution of maximum inversion table entry.
 * https://oeis.org/A056151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth entry of the triangle T(n,k) defined by T(n,0) =
 *            1, T(n,k) = k!·(k+1)^(n-k) – (k-1)!·k^(n-k+1) for 0 < k ≤ n. The
 *            sequence is obtained by reading the triangle row‑wise (1‑based
 *            index). The algorithm finds the row r and column k that
 *            correspond to the index n, then evaluates T(r,k)
 * Range:     u8 n <= 23, u16 n <= 41, u32 n <= 98, u64 n <= 243,
 *            u128 n <= 618, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;          /* a(1)=1 */

    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = (idx_t)((sqrt(8.0L * n + 1) - 1.0L) / 2.0L);
    if (r * (r + 1) / 2 < n) ++r;

    /* offset within the row (0‑based). */
    idx_t prev = (r - 1) * r / 2;          /* elements before row r */
    idx_t k = n - prev - 1;                /* column index in row r */

    /* Fixed‑base cases. */
    if (k == 0) return 1;

    /* Compute factorials up to r once per program run. */
    static seq_t fact[200];
    static idx_t fact_last = 0;
    if (fact_last == 0) {                  /* first call */
        fact[0] = 1;
        fact_last = 0;
    }
    if (r > fact_last) {
        for (idx_t i = fact_last + 1; i <= r; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_last = r;
    }

    /* Evaluate the two terms of T(r,k). */
    seq_t term1 = fact[k] * pow_seq((seq_t)(k + 1), (idx_t)(r - k));
    seq_t term2 = fact[k - 1] * pow_seq((seq_t)k, (idx_t)(r - k + 1));

    return term1 - term2;
}
