/*
 * A109047  a(n) = lcm(n, 6).
 * https://oeis.org/A109047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n, 6) = n * 6 / gcd(n, 6)
 * Range:     u8 n <= 42, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t six = (seq_t)6;
    seq_t g   = gcd_seq(n_t, six);
    return (n_t * six) / g;
}
