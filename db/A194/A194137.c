/*
 * A194137  a(n) = Sum_{j=1..n} floor(j*sqrt(6)); n-th partial sum of Beatty
 *          sequence for sqrt(6).
 * https://oeis.org/A194137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} floor(j*sqrt(6))
 * Range:     u8 n <= 14, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 128‑bit integers.
   Returns floor(sqrt(x)). */
static inline seq_t isqrt_u128(unsigned __int128 x) {
    unsigned __int128 r = 0;
    for (int b = 63; b >= 0; --b) {
        unsigned __int128 nxt = r | (((unsigned __int128)1) << b);
        if (nxt * nxt <= x) {
            r = nxt;
        }
    }
    return (seq_t)r;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        unsigned __int128 val = (unsigned __int128)6 * j * j;
        seq_t term = isqrt_u128(val);           /* floor(j*sqrt(6)) */
        sum += term;
    }
    return sum;
}
