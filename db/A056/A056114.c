/*
 * A056114  Expansion of (1+9*x)/(1-x)^11.
 * https://oeis.org/A056114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * binomial(n+9, 9)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 299, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute C(n+9, 9) = Π_{i=1}^9 (n+i)/i   */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 9; ++i) {
        seq_t num = (seq_t)(n + i);      /* numerator: n+i  */
        seq_t den = (seq_t)i;            /* denominator: i  */
        /* use 128‑bit intermediate to avoid overflow when seq_t is 64‑bit */
        binom = (seq_t)((unsigned __int128)binom * num / den);
    }

    /* Multiply by (n+1) to get the final term */
    seq_t mult = (seq_t)(n + 1);
    return (seq_t)((unsigned __int128)binom * mult);
}
