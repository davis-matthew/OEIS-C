/*
 * A146085  Positive integers a(n) such that for every integer m == 1 (mod 3),
 *          m >= 4, there exists a unique representation of m as a sum of the
 *          form a(l) + 3*a(s).
 * https://oeis.org/A146085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by taking n-1 in base 3, mapping each base‑3
 *            digit * to the next free position in the base‑3 expansion
 *            (positions 1,3,5,…); * the least significant digit is fixed to 1
 *            and all even positions ≥2 are * forced to 0
 * Range:     u8 n <= 12, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;            // offset starts at 1
    idx_t m = n - 1;                 // value to encode in the free positions
    seq_t result = 1;                // least significant digit fixed to 1
    if (m == 0) return result;
    seq_t pow = 3;                   // first free position is 3^1
    while (m > 0) {
        idx_t digit = m % 3;         // base‑3 digit for this free position
        result += (seq_t)digit * pow;
        m /= 3;
        pow *= 9;                    // jump two positions: multiply by 3^2
    }
    return result;
}
