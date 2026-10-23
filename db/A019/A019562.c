/*
 * A019562  Coordination sequence for C_6 lattice.
 * https://oeis.org/A019562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..6} P[k] * C(n - k + 5, 5) where P = {1, 66, 495,
 *            924, 495, 66, 1} is the expansion of
 *            (1+6x+x²)(1+60x+134x²+60x³+x⁴)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 47, u64 n <= 4043,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates a 5‑choose coefficient for n ≥ 5.  For n < 5 it returns 0. */
static seq_t binom5(idx_t n)
{
    if (n < 5u) return 0;
    seq_t r = 1;
    for (idx_t i = 0; i < 5; ++i) {
        r = r * (n - i);
        r = r / (i + 1);
    }
    return r;
}

seq_t a(idx_t n)
{
    static const unsigned P[7] = {1, 66, 495, 924, 495, 66, 1};
    seq_t sum = 0;
    /* iterate k only up to min(6, n) to avoid underflow on n-k */
    for (idx_t k = 0; k < 7 && k <= n; ++k) {
        idx_t m = n - k + 5;          /* m = n - k + 5  (always ≥ 5) */
        seq_t bin = binom5(m);
        sum += (seq_t)P[k] * bin;
    }
    return sum;
}
