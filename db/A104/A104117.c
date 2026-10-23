/*
 * A104117  For n=2^k, a(n) = k+1, else 0.
 * https://oeis.org/A104117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 + log₂(n) if n is a power of two, otherwise 0
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n ≥ 1; for n not a power of two, return 0 */
    if (n && ((n & (n - 1)) == 0)) {
        idx_t k;
#if defined(__GNUC__) || defined(__clang__)
        /* count trailing zeros – works on GCC/Clang */
        k = (idx_t)__builtin_ctzll(n);
#else
        /* portable fallback (slow for large n but acceptable for 64‑bit values) */
        k = 0;
        while ((n >>= 1)) ++k;
#endif
        return (seq_t)(k + 1);
    }
    return (seq_t)0;
}
