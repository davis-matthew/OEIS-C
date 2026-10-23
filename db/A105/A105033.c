/*
 * A105033  Read binary numbers downwards to the right.
 * https://oeis.org/A105033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - ∑{k ≥ 0 | 2^{k+1} ≤ n and n ≡ k (mod 2^{k+1})}
 *            2^{k+1}
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;          /* start with n itself */
    /* loop over k, with m = 2^{k+1} = 2ULL << k */
    for (idx_t k = 0; k < 63; ++k) {
        idx_t m = (idx_t)2ULL << k;   /* 2^{k+1} */
        if (m > n) break;            /* no further k possible */
        if (n % m == k) {
            res -= (seq_t)m;          /* subtract 2^{k+1} */
        }
    }
    return res;
}
