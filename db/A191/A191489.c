/*
 * A191489  Number of compositions of even natural numbers into 6 parts <= n.
 * https://oeis.org/A191489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + 1)^6 + (1+(-1)^n)/2 )/2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 43, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Convert to seq_t for arithmetic.
    seq_t p = (seq_t)(n) + (seq_t)1;   // p = n + 1

    // Compute p^6
    seq_t pow6 = p;
    for (int i = 0; i < 5; ++i) {
        pow6 *= p;
    }

    // (1 + (-1)^n) / 2 is 1 if n is even, 0 if n is odd.
    seq_t parity_term = ((n & (idx_t)1) == 0) ? (seq_t)1 : (seq_t)0;

    // Numerator = p^6 + parity_term
    seq_t numerator = pow6 + parity_term;

    // Final result: divide by 2
    return numerator >> 1;
}
