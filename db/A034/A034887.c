/*
 * A034887  Number of digits in 2^n.
 * https://oeis.org/A034887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(n*log10(2)) + 1
 * Range:     u8 n <= 847, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 64‑bit fixed‑point approximation of log10(2) with 18 decimal places:
     *  log10(2) ≈ 301029995663981195 / 10^18
     */
    const unsigned __int128 NUM = 301029995663981195ULL;   /* numerator   */
    const unsigned __int128 DEN = 1000000000000000000ULL;  /* 10^18       */

    unsigned __int128 prod = (unsigned __int128)n * NUM;
    seq_t q = (seq_t)(prod / DEN);

    return q + 1;
}
