/*
 * A106616  Numerator of n/(n+15).
 * https://oeis.org/A106616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, mult
 * Formula:   a(n) = n / gcd(n, 15) (with a(0)=0)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t gcd_seq_local(seq_t a, seq_t b)
{
    /* Euclidean algorithm: works for all unsigned integer types,
       including unsigned __int128 and arbitrary‑precision integer
       classes with operator % defined. */
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t g = gcd_seq_local(nn, (seq_t)15);
    return nn / g;
}
