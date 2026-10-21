/*
 * A210434  Number of digits in 4^n.
 * https://oeis.org/A210434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(n*log10(4)) + 1 * log10(4) = 0.602059991327962...
 *            (9‑digit decimal frac) * We use a fixed‑point rational
 *            602059991327962 / 1000000000000000 * which is accurate enough
 *            for all n ≤ 10⁴ (and many more). * The computation is entirely
 *            integer, no floating point or big‑int lib
 * Range:     u8 n <= 423, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 602059991327962 / 1000000000000000 ≈ log10(4) */
    static const unsigned long long NUM = 602059991327962ULL;
    static const unsigned long long DEN = 1000000000000000ULL; /* 10^15 */

    unsigned __int128 prod = (unsigned __int128)n * NUM;
    /* floor(n*log10(4)) = prod / DEN */
    seq_t digits = (seq_t)(prod / DEN + 1);
    return digits;
}
