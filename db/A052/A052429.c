/*
 * A052429  Least common multiple of nonzero digits of n.
 * https://oeis.org/A052429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = lcm of the non‑zero decimal digits of n
 * Range:     u8 n <= 577, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least common multiple of the non‑zero digits of n. */
seq_t a(idx_t n)
{
    seq_t lcm = 1;
    idx_t x = n;
    while (x > 0) {
        int d = x % 10;
        x /= 10;
        if (d == 0) continue;

        /* gcd computation by Euclid's algorithm */
        seq_t a1 = lcm, b1 = (seq_t)d;
        while (b1 != 0) {
            seq_t t = a1 % b1;
            a1 = b1;
            b1 = t;
        }
        seq_t g = a1;                     /* gcd(lcm, d) */
        lcm = lcm / g * (seq_t)d;         /* lcm = lcm * d / gcd */
    }
    return lcm;
}
