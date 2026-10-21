/*
 * A225593  The integer closest to n/e.
 * https://oeis.org/A225593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n/e) = floor(n/e + 0.5)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  e ≈ 2718281828459045235 / 1000000000000000000
    Hence  n/e ≈ n * 1000000000000000000 / 2718281828459045235.
    Adding half of the denominator before division gives the nearest integer. */

seq_t a(idx_t n)
{
    /* Safety for an out‑of‑bounds index */
    if (n == 0)
        return 0;

    /* 128‑bit constants for high precision */
    static const unsigned __int128 SCALE =
        (unsigned __int128)1000000000000000000ULL;          /* 10^18   */
    static const unsigned __int128 DEN =
        (unsigned __int128)2718281828459045235ULL;          /* floor(e * 10^18) */

    unsigned __int128 num = (unsigned __int128)n * SCALE + DEN / 2;
    seq_t result = (seq_t)(num / DEN);
    return result;
}
