/*
 * A171441  Expansion of g.f. (1+x)^6/(1-x).
 * https://oeis.org/A171441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(7, n-2k)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed binomial coefficients C(7, t) for t = 0..7 */
    static const seq_t binom[8] = {1, 7, 21, 35, 35, 21, 7, 1};

    seq_t sum = 0;
    /* Sum the terms with the same parity as n, stopping at t=7 or n */
    for (idx_t t = n & 1; t <= 7 && t <= n; t += 2)
        sum += binom[t];

    return sum;
}
