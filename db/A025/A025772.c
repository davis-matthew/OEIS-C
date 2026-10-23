/*
 * A025772  Expansion of 1/((1-x)*(1-x^4)*(1-x^5)).
 * https://oeis.org/A025772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((n + 5)^2 + 15) / 40) + [(n mod 20) = 5]
 * Range:     u8 n <= 10, u16 n <= 265, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid accidental promotion to *smaller* type */
    seq_t n_t = (seq_t)n;

    /* Compute (n + 5)^2 + 15 */
    seq_t np5 = n_t + 5;
    seq_t sq   = np5 * np5;
    seq_t num  = sq + 15;

    /* Integer division gives the floor for positive numbers */
    seq_t res  = num / 40;

    /* Add 1 if n mod 20 equals 5 */
    if ((n % 20) == 5U)
        ++res;

    return res;
}
