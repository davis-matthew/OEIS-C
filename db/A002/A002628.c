/*
 * A002628  Number of permutations of length n without 3-sequences.
 * https://oeis.org/A002628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,k)*(d(n-k) + d(n-k-1))
 *            for n>0, a(0) = 1, where d(j) = A000166(j): d(0) = 1, d(1) = 0,
 *            d(j+1) = j*(d(j) + d(j-1))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;

    idx_t k0 = n / 2;
    idx_t j0 = n - k0;

    /* c = binomial(j0, k0), computed multiplicatively (each step exact) */
    seq_t c = 1;
    for (idx_t i = 0; i < k0; i++) {
        c = c * (seq_t)(j0 - i) / (seq_t)(i + 1);
    }

    seq_t dprev = 1; /* d(j-1), starting at j = 1: d(0) */
    seq_t dcur = 0;  /* d(j),   starting at j = 1: d(1) */
    seq_t result = 0;

    for (idx_t j = 1; j <= n; j++) {
        if (j >= j0) {
            /* term for k = n - j, c = binomial(j, k) */
            result += c * (dcur + dprev);
            if (j < n) {
                idx_t k = n - j;
                /* binomial(j+1, k-1) = binomial(j,k) * (j+1) * k / ((j-k+1)(j-k+2)) */
                c = c * (seq_t)(j + 1) * (seq_t)k
                    / ((seq_t)(j - k + 1) * (seq_t)(j - k + 2));
            }
        }
        /* advance: d(j+1) = j*(d(j) + d(j-1)) */
        seq_t dnext = (seq_t)j * (dcur + dprev);
        dprev = dcur;
        dcur = dnext;
    }

    return result;
}
