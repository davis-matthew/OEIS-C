/*
 * A129196  a(n) = denominator(3*(3+(-1)^n)/(n+1)^3).
 * https://oeis.org/A129196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator(3*(3+(-1)^n)/(n+1)^3)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* O(1) computation using gcd */

/* gcd for seq_t */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* v = n+1 as seq_t */
    seq_t v = (seq_t)n + (seq_t)1;

    /* compute v^3 = v * v * v */
    seq_t v2 = v * v;
    seq_t v3 = v2 * v;

    /* compute gcd with 2 and 3 */
    seq_t d2 = seq_gcd(v, (seq_t)2);
    seq_t d3 = seq_gcd(v, (seq_t)3);

    /* denominator = v^3 / (d2*d3) */
    return v3 / (d2 * d3);
}
