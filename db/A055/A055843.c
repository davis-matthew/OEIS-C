/*
 * A055843  Expansion of (1+3*x)/(1-x)^10.
 * https://oeis.org/A055843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*n+9)*binomial(n+8,8)/9
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 37, u64 n <= 487, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 1;

    seq_t ans = 1;
    for (idx_t k = 1; k <= n; ++k) {
#ifdef __cplusplus
        /* If seq_t is 64‑bit, use unsigned __int128 for the
           intermediate product to avoid overflow; otherwise
           rely on the arbitrary‑precision arithmetic of seq_t. */
        if constexpr (sizeof(seq_t) <= 8) {
            unsigned __int128 tmp = (unsigned __int128)ans * (k + 8) * (4 * k + 9);
            tmp /= (k * (4 * k + 5));
            ans = (seq_t)tmp;
        } else {
            seq_t tmp = ans * (seq_t)(k + 8) * ((seq_t)(4 * k + 9));
            tmp /= ((seq_t)k * ((seq_t)(4 * k + 5)));
            ans = tmp;
        }
#else
        unsigned __int128 tmp = (unsigned __int128)ans * (k + 8) * (4 * k + 9);
        tmp /= (k * (4 * k + 5));
        ans = (seq_t)tmp;
#endif
    }
    return ans;
}
