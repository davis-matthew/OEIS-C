/*
 * A053831  Sum of digits of n written in base 11.
 * https://oeis.org/A053831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(0) = 0; a(n) = a(n/11) + (n mod 11) for n > 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of the digits of n written in base 11. */
seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t t = n;
    while (t > 0) {
        sum += (seq_t)(t % 11);
        t /= 11;
    }
    return sum;
}
