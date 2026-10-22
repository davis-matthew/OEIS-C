/*
 * A004504  Tersum n + 15.
 * https://oeis.org/A004504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tersum(n, 15): add the base-3 digits of n and 15 mod 3,
 *            with no carries
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    seq_t x = (seq_t)n;
    unsigned m = 15;

    while (x > 0 || m > 0) {
        seq_t d = (x % 3 + (seq_t)(m % 3)) % 3;
        result += d * power;
        power *= 3;
        x /= 3;
        m /= 3;
    }

    return result;
}
