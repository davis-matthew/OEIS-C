/*
 * A002380  a(n) = 3^n reduced modulo 2^n.
 * https://oeis.org/A002380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 2^n
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 65, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* mod = 2^n; in a fixed-width build it wraps to 0 once n >= width,
       in which case natural wraparound already reduces mod 2^width,
       which agrees with 3^n mod 2^n whenever that value fits. */
    seq_t mod = 1;
    for (idx_t i = 0; i < n; i++) {
        mod = mod * 2;
        if (mod == 0) break;
    }

    seq_t x = 1;
    for (idx_t i = 0; i < n; i++) {
        x = x * 3;
        if (mod != 0) x = x % mod;
    }
    if (mod != 0) x = x % mod;
    return x;
}
