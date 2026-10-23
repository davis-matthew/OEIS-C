/*
 * A013757  a(n) = 15^(3*n + 2).
 * https://oeis.org/A013757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15^(3*n + 2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 4, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the recurrence a(n) = 3375 * a(n-1), a(0) = 225,
       but compute it with binary exponentiation to avoid
       O(n) multiplication for large n. */
    seq_t result = 225;           /* 15^(3*0 + 2) */
    if (n == 0) return result;

    seq_t factor = 3375;          /* 15^3 */
    idx_t e = n;                  /* exponent for the factor */

    seq_t power = factor;
    while (e > 0) {
        if (e & 1) {
            result = result * power;
        }
        e >>= 1;
        if (e) {
            power = power * power;
        }
    }
    return result;
}
