/*
 * A003136  Loeschian numbers: numbers of the form x^2 + xy + y^2; norms of
 *          vectors in A2 lattice.
 * https://oeis.org/A003136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  core, easy, nonn, nice
 * Formula:   n is in the sequence iff for each prime p ≡ 2 (mod 3) dividing
 *            n, the exponent is even
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9649 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int is_loeschian(seq_t m) {
    if (m == 0) return 1;
    
    seq_t n = m;
    
    for (seq_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            if (p % 3 == 2) {
                // p ≡ 2 (mod 3), check if exponent is even
                seq_t exp = 0;
                while (n % p == 0) {
                    exp++;
                    n /= p;
                }
                if (exp % 2 != 0) {
                    return 0;
                }
            } else {
                // p ≡ 0 or 1 (mod 3), just divide out
                while (n % p == 0) {
                    n /= p;
                }
            }
        }
    }
    
    // Check if remaining factor > 1 is a prime ≡ 2 (mod 3)
    if (n > 1 && n % 3 == 2) {
        return 0;
    }
    
    return 1;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t val = 0;
    
    while (1) {
        if (is_loeschian(val)) {
            count++;
            if (count == n) {
                return val;
            }
        }
        val++;
    }
}
