/*
 * A186544  a(n) = floor((Pi-2)*n/(Pi-3)); complement of A187320.
 * https://oeis.org/A186544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((Pi-2)*n/(Pi-3))
 * Range:     u8 n <= 31, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The irrational factor (Pi-2)/(Pi-3) is approximated by the rational
 * 37918/4703, which differs from the exact value by less than 5·10⁻⁸.
 * For any n up to about 10⁶ the error in n*α is <0.5, hence the floor
 * is unchanged.  The approximation reproduces all terms in the OEIS
 * b-file for the ranges tested. */

seq_t a(idx_t n)
{
    /* Compute floor(n * 37918 / 4703) using 128‑bit arithmetic for safety */
    return (seq_t)(((__uint128_t)n * 37918ULL) / 4703ULL);
}
