/*
 * A130250  Minimal index k of a Jacobsthal number such that A001045(k) >= n
 *          (the 'upper' Jacobsthal inverse).
 * https://oeis.org/A130250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌈log₂(3n − 1)⌉ for n ≥ 1, a(0) = 0
 * Range:     u8 n <= 85, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 117 terms through n = 4971 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the ceiling of log₂ of a positive seq_t value */
static seq_t ceil_log2_seq_t(seq_t x)
{
    if (x <= (seq_t)1u)                 /* log₂(1)=0, log₂(0) undefined -> 0 */
        return (seq_t)0u;

    /* ceil(log₂(x)) = floor(log₂(x−1)) + 1 */
    seq_t t   = x - (seq_t)1u;
    idx_t log = 0;
    while (t > (seq_t)0u) {
        t >>= 1;
        log++;
    }
    return (seq_t)log;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0u;

    /* val = 3*n - 1, computed in seq_t to avoid overflow on large idx_t */
    seq_t val = (seq_t)n * (seq_t)3u - (seq_t)1u;
    return ceil_log2_seq_t(val);
}
