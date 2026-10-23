/*
 * A124431  a(n) = Sum_{k=0..n} 2^k*C([(n+k)/2],k)*C([(n+k+1)/2],k) where
 *          [x]=floor(x).
 * https://oeis.org/A124431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(floor((n+k)/2), k) * * *
 *            binomial(floor((n+k+1)/2), k) (OEIS A124431)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Initial binomial values for k = 0 */
    idx_t prevA = n / 2;          /* floor((n+0)/2) */
    idx_t prevB = (n + 1) / 2;    /* floor((n+0+1)/2) */
    seq_t bin1 = 1;               /* binom(prevA, 0) */
    seq_t bin2 = 1;               /* binom(prevB, 0) */

    seq_t sum   = 0;              /* accumulator for the sum */
    seq_t pow2  = 1;              /* 2^k, starts at 1 for k = 0 */

    /* k = 0 term */
    sum += pow2 * bin1 * bin2;

    for (idx_t k = 1; k <= n; ++k) {
        /* update power of two */
        pow2 <<= 1;

        /* new values of floor((n+k)/2) and floor((n+k+1)/2) */
        idx_t currA = (n + k) / 2;
        idx_t currB = (n + k + 1) / 2;

        /* ----------- update binom(prevA, k) ------------ */
        if (currA == prevA) {
            /* A stays the same: binom(A, k) = binom(A, k-1) * (A - k + 1) / k */
            seq_t factor = (seq_t)(prevA - k + 1);
            bin1 = bin1 * factor / (seq_t)k;
        } else {
            /* A increases by 1: binom(A+1, k) = binom(A, k) + binom(A, k-1) */
            seq_t temp = bin1 * (seq_t)(prevA - k + 1) / (seq_t)k; /* binom(A, k) */
            bin1 = temp + bin1;                                    /* + binom(A, k-1) */
        }

        /* ----------- update binom(prevB, k) ------------ */
        if (currB == prevB) {
            seq_t factor = (seq_t)(prevB - k + 1);
            bin2 = bin2 * factor / (seq_t)k;
        } else {
            seq_t temp = bin2 * (seq_t)(prevB - k + 1) / (seq_t)k;
            bin2 = temp + bin2;
        }

        /* add current term to the sum */
        sum += pow2 * bin1 * bin2;

        /* prepare for next iteration */
        prevA = currA;
        prevB = currB;
    }

    return sum;
}
