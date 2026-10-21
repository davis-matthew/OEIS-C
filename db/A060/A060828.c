/*
 * A060828  Size of the Sylow 3-subgroup of the symmetric group S_n.
 * https://oeis.org/A060828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 ^ ( floor(n/3) + floor(n/9) + floor(n/27) + … )
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 44, u64 n <= 83, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute the 3‑adic exponent e(n) = Σ floor(n / 3^k) */
    idx_t e = 0;
    idx_t d = 3;

    /* avoid overflow: stop when d > n / 3 or d > n */
    while (d <= n) {
        e += n / d;
        if (d > n / 3)      /* next multiplication would overflow or exceed n */
            break;
        d *= 3;
    }

    /* compute 3^e using exponentiation by squaring, result type seq_t */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;

    while (e) {
        if (e & 1)
            result = result * base;
        base = base * base;
        e >>= 1;
    }

    return result;
}
