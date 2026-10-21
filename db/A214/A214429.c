/*
 * A214429  Integers of the form (k^2 - 49) / 120.
 * https://oeis.org/A214429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (k^2 - 49)/120 with k = 30*n + offset depending on n%4.
 *            Equivalent quasi-polynomial: a(4*n) = n*(15*n + 7)/2 a(4*n+1) =
 *            (3*n + 2)*(5*n + 1)/2 a(4*n+2) = (3*n + 1)*(5*n + 4)/2 a(4*n+3)
 *            = (n + 1)*(15*n + 8)/2
 * Range:     u8 n <= 23, u16 n <= 373, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t q = (seq_t)(n >> 2);
    switch (n & 3) {
        case 0:
            return q * ((seq_t)15 * q + 7) / 2;
        case 1:
            return ((seq_t)3 * q + 2) * ((seq_t)5 * q + 1) / 2;
        case 2:
            return ((seq_t)3 * q + 1) * ((seq_t)5 * q + 4) / 2;
        case 3:
            return (q + 1) * ((seq_t)15 * q + 8) / 2;
    }
    return 0; /* unreachable */
}
