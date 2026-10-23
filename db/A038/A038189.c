/*
 * A038189  Bit to left of least significant 1-bit in binary expansion of n.
 * https://oeis.org/A038189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n >> (ctz(n)+1)) & 1, with a(0)=0 This implements the
 *            definition: "bit to left of least significant 1-bit"
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* count trailing zeros of n */
#if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
    /* Builtin works for 64‑bit unsigned integers */
    /* use the GCC/Clang version if available */
#if defined(__GNUC__) || defined(__clang__)
    __builtin_ctzll(n); /* ensure it is compiled as builtin */
#endif
    idx_t t = __builtin_ctzll(n);
#else
    /* portable fallback */
    idx_t t = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
        ++t;
    }
#endif

    idx_t s = t + 1;                    /* shift amount */
    const idx_t bitlen = sizeof(seq_t) * 8;

    if (s >= bitlen)                     /* shift beyond type width → 0 */
        return 0;

    return ((seq_t)n >> s) & 1;           /* result is 0 or 1 */
}
