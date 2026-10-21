/*
 * A089262  a(n) = 2^floor(log_2(n)) - 2^floor(log_2(n*2/3)).
 * https://oeis.org/A089262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^⌊log₂(n)⌋ – 2^⌊log₂(⌊2n/3⌋)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute ⌊log₂(v)⌋ for unsigned integer v (v≥0). 0→0 the same as OEIS */
static inline idx_t floor_log2_idx(idx_t v) {
    if (v <= 1) return 0;
#if defined(__GNUC__) || defined(__clang__)
    /* __builtin_clzll is defined for unsigned long long, which covers idx_t */
    return (sizeof(idx_t)*8 - 1) - __builtin_clzll(v);
#else
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
#endif
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* a(0) = 0 */

    /* k1 = ⌊log₂(n)⌋ */
    idx_t k1 = floor_log2_idx(n);

    /* m = ⌊(2·n)/3⌋  using 128‑bit to avoid overflow */
    unsigned __int128 prod = (unsigned __int128)n * 2;
    idx_t m = (idx_t)(prod / 3);

    /* k2 = ⌊log₂(m)⌋ ; for m==0 this is 0, making 2⁰ = 1 which is 0 for n<3 */
    idx_t k2 = (m == 0) ? 0 : floor_log2_idx(m);

    /* 2ᵏⁱ as seq_t values */
    seq_t val1 = (seq_t)1 << k1;
    seq_t val2 = (seq_t)1 << k2;

    return val1 - val2;
}
