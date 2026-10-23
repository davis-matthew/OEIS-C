/*
 * A106610  Numerator of n/(n+9).
 * https://oeis.org/A106610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, mult
 * Formula:   a(n) = n / gcd(n,9) (the numerator of n/(n+9))
 * Range:     u8 n <= 255, u16 n <= 9398, u32 n <= 9398, u64 n <= 9398,
 *            u128 n <= 9398, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t g = seq_gcd(N, (seq_t)9);
    return N / g;
}
