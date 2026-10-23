/*
 * A179535  a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(n-k,k)^2 * 81^k.
 * https://oeis.org/A179535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(n-k,k)^2 * 81^k
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum   = 0;
    seq_t Cprev = 1;    /* binomial(n,0) */
    seq_t aPrev = 1;    /* binomial(n-0,0) */
    seq_t p     = 1;    /* 81^0 */
    idx_t half = n / 2;

    for (idx_t k = 0; k <= half; ++k) {
        seq_t t1 = Cprev * aPrev;          /* C(n,k) * binom(n-k,k) */
        seq_t term = t1 * t1;              /* square */
        term = term * p;                   /* times 81^k */
        sum += term;

        if (k == half) break;              /* further terms are zero */

        /* Update Cprev to binomial(n,k+1) */
        Cprev = (Cprev * (seq_t)(n - k)) / (seq_t)(k + 1);

        /* Update aPrev to binomial(n-(k+1),k+1) */
        seq_t num = (seq_t)(n - 2 * (k + 1) + 2) *
                    (seq_t)(n - 2 * (k + 1) + 1);
        seq_t den = (seq_t)(k + 1) *
                    (seq_t)(n - (k + 1) + 1);
        aPrev = (aPrev * num) / den;

        /* Update power of 81 */
        p *= (seq_t)81;
    }
    return sum;
}
