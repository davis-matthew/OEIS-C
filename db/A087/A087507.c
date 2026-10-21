/*
 * A087507  #{0<=k<=n: k*n is divisible by 3}.
 * https://oeis.org/A087507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + 1 if n ≡ 0 (mod 3) Formula: a(n) = ⌊n/3⌋ + 1
 *            otherwise
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n % 3) == 0) {
        return (seq_t)n + 1ULL;
    } else {
        return (seq_t)(n / 3) + 1ULL;
    }
}
