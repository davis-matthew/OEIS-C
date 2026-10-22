/*
 * A003662  a(n) is smallest number != a(j) + a(k), j < k and a(1) = 1, a(2) =
 *          4.
 * https://oeis.org/A003662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th positive integer m with m = 8 or m ≡ 1, 4, 6,
 *            11 (mod 12)
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t m = 0;
    seq_t target = (seq_t)n;
    
    while (count < target) {
        m++;
        seq_t rem = m % 12;
        if (m == 8 || rem == 1 || rem == 4 || rem == 6 || rem == 11) {
            count++;
        }
    }
    
    return m;
}
