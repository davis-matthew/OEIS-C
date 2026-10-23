/*
 * A191637  a(n) = floor((1 + 4^n)/(1 + 2*n)).
 * https://oeis.org/A191637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1 + 4^n)/(1 + 2*n))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* denominator 1 + 2*n */
    seq_t den = (seq_t)(2*n) + 1;

#ifdef __SIZEOF_INT128__
    /* For n <= 63 we can compute 4^n in a 128‑bit intermediate
       and avoid overflow of the 64‑bit seq_t type. */
    if (n <= 63) {
        unsigned __int128 num = ((unsigned __int128)1 << (2*n)) + 1;
        unsigned __int128 div = (unsigned __int128)den;
        seq_t res = (seq_t)(num / div);
        return res;
    }
#endif

    /* If seq_t is 64 bits (or less) and n > 63 the result would
       overflow the 64‑bit limits; the build is only required to
       produce correct values that fit in seq_t, so return 0 here. */
    if (sizeof(seq_t) <= 8 && n > 63) {
        return 0;
    }

    /* General case: arbitrary‑precision seq_t or n > 63 with large
       type. Compute 4^n by repeated multiplication. */
    seq_t num = 1;
    for (idx_t i = 0; i < n; ++i) {
        num *= 4;
    }
    num += 1;
    return num / den;
}
