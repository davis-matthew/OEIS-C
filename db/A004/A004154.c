/*
 * A004154  a(n) = n! with trailing zeros omitted.
 * https://oeis.org/A004154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000142(n) / 10^A027868(n), i.e. n! with z = sum_{k>=1}
 *            floor(n/5^k) factors of 2 and of 5 removed; computed as
 *            prod_{i=1..n} i with all 5s stripped and the first z factors of
 *            2 stripped greedily, so every partial product divides a(n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t z = 0, t = n;
    while (t > 0) {
        t /= 5;
        z += t;
    }
    seq_t result = 1;
    for (idx_t i = 2; i <= n; i++) {
        idx_t m = i;
        while (m % 5 == 0) m /= 5;
        while (z > 0 && m % 2 == 0) {
            m /= 2;
            z--;
        }
        result *= (seq_t)m;
    }
    return result;
}
