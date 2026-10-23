/*
 * A177350  Irregular triangle read by rows: T(n, k) = Sum_{m=1..floor(n/2)+1}
 *          binomial(n - (m + k), m + k).
 * https://oeis.org/A177350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is the n‑th element of the irregular triangle read by rows.
 *            T(r,k) = Σ_{m=1}^{⌊r/2⌋+1} binomial(r-(m+k), m+k) where
 *            k∈[-⌊r/2⌋,⌊r/2⌋]. The sequence a(n) is obtained by concatenating
 *            all rows r=0,1,2,… of T. This implementation computes a(n) by
 *            locating the row r and offset within that row for...
 * Range:     i8 n <= 59, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient for integer n (possibly negative) and non‑negative k.
   Uses the multiplicative formula nCk = n*(n-1)*…*(n-k+1)/k!.
   The result is exact for all integer n and non‑negative k. */
static seq_t binom_int(seq_t N, idx_t K)
{
    if (K == 0) return 1;
    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = N - (seq_t)(K - i);   /* n-(i-1) */
        result = result * num / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r and offset within that row for index n. */
    idx_t idx = n;
    idx_t r = 0;
    while (1) {
        idx_t f = r / 2;
        idx_t rowlen = 2 * f + 1;
        if (idx < rowlen) break;
        idx -= rowlen;
        ++r;
    }
    idx_t f = r / 2;
    idx_t offset = idx;
    long long k = (long long)offset - (long long)f;   /* k may be negative */

    /* Compute T(r,k).  Sum over m = max(1,-k) .. f+1. */
    idx_t m_start = 1;
    if (k < 0) {
        long long temp = -k;
        if (temp > (long long)m_start) m_start = (idx_t)temp;
    }
    idx_t m_end = f + 1;
    seq_t sum = 0;
    for (idx_t m = m_start; m <= m_end; ++m) {
        long long m_plus_k = (long long)m + k;
        if (m_plus_k < 0) continue;
        seq_t term_n = (seq_t)((long long)r - m_plus_k);
        seq_t term_k = (seq_t)m_plus_k;
        seq_t val = binom_int(term_n, (idx_t)term_k);
        sum += val;
    }
    return sum;
}
