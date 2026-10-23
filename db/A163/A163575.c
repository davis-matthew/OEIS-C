/*
 * A163575  Remove all trailing bits equal to (n mod 2) in binary
 *          representation of n.
 * https://oeis.org/A163575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 512, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the integer obtained by removing all trailing bits equal to n mod 2
 * from the binary representation of n. */
seq_t a(idx_t n) {
    idx_t parity = n & 1U;
    while (n != 0 && (n & 1U) == parity) {
        n >>= 1U;
    }
    return (seq_t)n;
}
