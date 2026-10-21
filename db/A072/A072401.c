/*
 * A072401  1 iff n is of the form 4^m*(8k+7).
 * https://oeis.org/A072401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 4^m * (8k+7) for some integers m,k ≥ 0;
 *            otherwise a(n) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0; /* 0 is representable as 0^2+0^2+0^2 */
    }
    /* Remove all factors of 4 */
    while ((n % 4) == 0) {
        n /= 4;
    }
    /* n is now odd; check for ≡ 7 (mod 8) */
    return (n % 8) == 7 ? 1 : 0;
}
