/*
 * A085357  Common residues of binomial(3n,n)/(2n+1) modulo 2: relates ternary
 *          trees (A001764) to the infinite Fibonacci word (A003849).
 * https://oeis.org/A085357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if binomial(3*n, n) is odd; equivalently n & (n<<1) ==
 *            0, else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses Lucas's theorem for parity of binomial coefficients. */
seq_t a(idx_t n)
{
    /* n is uint64_t; shifting left 1 drops overflow bits, which is harmless for parity. */
    return (seq_t)(((n & (n << 1)) == 0) ? 1 : 0);
}
