/*
 * A171442  Expansion of (1+x)^7/(1-x).
 * https://oeis.org/A171442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(8, n-2*k)
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed binomial coefficients C(8, r) for r = 0..8 */
    static const seq_t binom[9] = {1, 8, 28, 56, 70, 56, 28, 8, 1};

    seq_t result = 0;
    idx_t max_k = n / 2;               /* floor(n/2) */

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t r = n - (k << 1);        /* r = n - 2*k, never underflows */
        if (r <= 8) {
            result += binom[r];
        }
    }
    return result;
}
