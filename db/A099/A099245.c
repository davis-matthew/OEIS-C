/*
 * A099245  Numerator of relative frequency of number of ones in the binary
 *          representation of n.
 * https://oeis.org/A099245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, base
 * Formula:   a(n) = popcount(n) / GCD( floor(log2(n)) + 1 , popcount(n) ),
 *            a(0) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* popcount of n */
    idx_t pop = __builtin_popcountll(n);

    /* compute number of bits needed to represent n: floor(log2(n)) + 1 */
    idx_t bits = 0;
    idx_t t = n;
    while (t) {
        bits++;
        t >>= 1;
    }

    /* GCD of pop and bits (Euclidean algorithm) */
    idx_t a1 = pop, b1 = bits;
    while (b1 != 0) {
        idx_t r = a1 % b1;
        a1 = b1;
        b1 = r;
    }
    idx_t g = a1;          /* g = GCD(pop, bits) */

    return (seq_t)(pop / g);
}
