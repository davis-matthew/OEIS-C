/*
 * A223568  Number of nX7 0..1 arrays with antidiagonals unimodal.
 * https://oeis.org/A223568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1}^{min(n,7)-1} (1 + i(i+1)/2)^2 * * (1 +
 *            min(n,7)(min(n,7)+1)/2)^{max(n,7)-min(n,7)+1}
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = (n < 7 ? n : 7);          /* min(n,7) */

    seq_t prod = 1;

    /* product over i = 1 .. m-1 */
    for (idx_t i = 1; i < m; ++i) {
        seq_t tri   = (seq_t)i * (i + 1) / 2;  /* i*(i+1)/2 */
        seq_t term  = 1 + tri;                /* 1 + tri */
        prod = prod * term * term;           /* square and multiply */
    }

    /* base for the exponent part */
    seq_t base = 1 + (seq_t)m * (m + 1) / 2;

    /* exponent = max(n,7) - m + 1 */
    idx_t exp = (n >= 7 ? n - 6 : 8 - m);

    for (idx_t k = 0; k < exp; ++k) {
        prod = prod * base;
    }

    return prod;
}
