/*
 * A059565  Beatty sequence for e^gamma (gamma is the Euler-Mascheroni
 *          constant A001620).
 * https://oeis.org/A059565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * e^γ) (Euler–Mascheroni constant γ)
 * Range:     u8 n <= 143, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* high‑precision constant for e^γ ≈ 1.7810724179901979852365 */
    static const long double alpha = 1.7810724179901979852365L;
    /* use integer arithmetic to compute the floor */
    unsigned long long val = (unsigned long long)((long double)n * alpha);
    return (seq_t)val;
}
