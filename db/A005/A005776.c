/*
 * A005776  Exponents m_i associated with Weyl group W(E_8).
 * https://oeis.org/A005776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th integer k in [1,30) with gcd(k,30) = 1 (search over
 *            k)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t count = 0;
    unsigned k;
    for (k = 1; k < 30; k++) {
        unsigned x = k, y = 30;
        while (y) { unsigned t = x % y; x = y; y = t; }
        if (x == 1) {
            count++;
            if (count == n) return (seq_t)k;
        }
    }
    return (seq_t)0;
}
