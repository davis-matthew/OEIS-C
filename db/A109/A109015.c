/*
 * A109015  a(n) = gcd(n,12).
 * https://oeis.org/A109015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = gcd(n,12)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* gcd(n,12) = gcd(n % 12, 12) – 12 is small enough to fit in seq_t */
    idx_t m = n % 12;
    seq_t a_val = (seq_t)m;
    seq_t b_val = (seq_t)12;
    while (b_val != (seq_t)0) {
        seq_t r = a_val % b_val;
        a_val = b_val;
        b_val = r;
    }
    return a_val;
}
