/*
 * A060236  If n mod 3 = 0 then a(n) = a(n/3), otherwise a(n) = n mod 3.
 * https://oeis.org/A060236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n mod 3 if n mod 3 != 0, otherwise a(n) = a(n/3).
 *            Equivalently, a(3*k) = a(k), a(3*k+1) = 1, a(3*k+2) = 2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1; safe guard */
    while ((n % (idx_t)3) == 0) { /* strip factors of 3 */
        n /= 3;
    }
    return (seq_t)(n % (idx_t)3);
}
