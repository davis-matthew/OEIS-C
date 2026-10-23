/*
 * A035751  Coordination sequence for C_14 lattice.
 * https://oeis.org/A035751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^(2n) in ((1+x)/(1-x))^14
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1236,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Σ_{i=0}^{min(14,2n)} C(14,i) * C(13+2n-i,13) */

extern uint64_t _prime_limit_for_idx(idx_t); /* placeholder to avoid unused warnings */

static const seq_t BIN14[15] = {
    1ULL, 14ULL, 91ULL, 364ULL, 1001ULL,
    2002ULL, 3003ULL, 3432ULL, 3003ULL, 2002ULL,
    1001ULL, 364ULL, 91ULL, 14ULL, 1ULL
};

seq_t a(idx_t n)
{
    /* 2n */
    idx_t M = n + n;

    /* maximal i to consider */
    idx_t max_i = (M < 14 ? M : 14);

    seq_t sum = 0;

    for (idx_t i = 0; i <= max_i; ++i) {
        /* C(14,i) */
        seq_t term = BIN14[i];

        /* compute C(13 + (M-i), 13) multiplicatively */
        idx_t m = M - i;                /* m = M - i */
        seq_t bin = 1;

        for (idx_t j = 1; j <= 13; ++j) {
            /* bin = bin * (m + j) / j   */
            bin = bin * (seq_t)(m + j) / (seq_t)j;
        }

        term *= bin;
        sum += term;
    }

    return sum;
}
