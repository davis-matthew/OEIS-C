/*
 * A037462  a(n) = Sum_{i = 0..m} d(i)*8^i, where Sum_{i = 0..m} d(i)*4^i is
 *          the base 4 representation of n.
 * https://oeis.org/A037462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{i≥0} d_i·8^i, where n = Σ_{i≥0} d_i·4^i (base‑4
 *            digits)
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(n)=8·a(n/4) if n≡0 (mod 4), else a(n)=a(n−1)+1 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t factor = 1;

    while (n) {
        unsigned digit = n & 3;   /* n % 4 */
        result += (seq_t)digit * factor;
        n >>= 2;                 /* n /= 4 */
        factor *= (seq_t)8;
    }

    return result;
}
