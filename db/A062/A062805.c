/*
 * A062805  a(n) = Sum_{i=1..n} i*n^(n-i).
 * https://oeis.org/A062805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n^(n+1) - n^2) / (n-1)^2
 * (n = 0 is allowed; a(0) = 0, a(1) = 1)
 */
static seq_t pow_big(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Two modes: 64‑bit build (or smaller) uses 128‑bit intermediates;
     * wider seq_t (big‑int) uses the exact formula with big integers. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum = 0;
        for (idx_t i = 1; i <= n; ++i) {
            /* Compute n^(n-i) using 128‑bit arithmetic */
            unsigned __int128 power = 1;
            unsigned __int128 base = n;
            idx_t exp = n - i;
            while (exp) {
                if (exp & 1) power *= base;
                base *= base;
                exp >>= 1;
            }
            sum += (unsigned __int128)i * power;
        }
        return (seq_t)sum;
    } else {
        seq_t base = (seq_t)n;
        seq_t power = pow_big(base, n + 1);          /* n^(n+1)  */
        seq_t nseq = (seq_t)n;
        seq_t numerator = power - nseq * nseq;      /* n^(n+1) - n^2 */
        seq_t denom = (seq_t)(n - 1) * (seq_t)(n - 1); /* (n-1)^2   */
        return numerator / denom;
    }
}
