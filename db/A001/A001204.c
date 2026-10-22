/*
 * A001204  Continued fraction for e^2.
 * https://oeis.org/A001204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr, nice
 * Formula:   For n > 0: a(5n) = 12n + 6, a(5n+1) = 3n + 2, a(5n+2) = a(5n+3)
 *            = 1, a(5n+4) = 3n + 3. Special values: a(0) = 7, a(1) = 2, a(2)
 *            = 1, a(3) = 1
 * Range:     u8 n <= 104, u16 n <= 9181, u32 n <= 9181, u64 n <= 9181,
 *            u128 n <= 9181, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 7;
    if (n == 1) return 2;
    if (n == 2) return 1;
    if (n == 3) return 1;
    
    seq_t q = (seq_t)(n / 5);
    idx_t r = n % 5;
    
    switch (r) {
        case 0: return 12 * q + 6;
        case 1: return 3 * q + 2;
        case 2:
        case 3: return 1;
        case 4: return 3 * q + 3;
    }
    return 0;
}
