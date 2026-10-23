/*
 * A041689  Denominators of continued fraction convergents to sqrt(364).
 * https://oeis.org/A041689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1; a(1)=12; a(n) = c[(n-1)%12] * a(n-1) + a(n-2) for n ≥ 2,
 *            where c = {12,1,2,3,1,8,1,3,2,1,12,38}
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    /* Periodic coefficients of the continued fraction of √364. */
    static const seq_t coeff[12] = {
        (seq_t)12, (seq_t)1, (seq_t)2, (seq_t)3,
        (seq_t)1,  (seq_t)8, (seq_t)1, (seq_t)3,
        (seq_t)2,  (seq_t)1, (seq_t)12, (seq_t)38
    };

    seq_t prev2 = (seq_t)0;  /* a(-1) – denominator of the “right‑hand” precedent. */
    seq_t prev1 = (seq_t)1;  /* a(0). */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t c = coeff[(k - 1) % 12];  /* coefficient a_k. */
        seq_t cur = c * prev1 + prev2; /* recurrence for a_k. */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
