/*
 * A047430  Numbers that are congruent to {0, 4, 5, 6} mod 8.
 * https://oeis.org/A047430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * ((n-1)/4) + {0,4,5,6}[(n-1)%4]
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n=0; guard against accidental use */
        return (seq_t)0;

    seq_t n_t        = (seq_t)n;
    seq_t n_minus_1  = n_t - 1;
    seq_t q          = n_minus_1 / 4;   /* integer division */
    seq_t r          = n_minus_1 % 4;   /* remainder 0..3 */
    seq_t base       = q * 8;           /* 8 * floor((n-1)/4) */

    /* Choose the residue based on the remainder */
    int r_int = (int)r;                 /* safe: r is 0..3 */
    switch (r_int) {
        case 0: return base;            /* residue 0 */
        case 1: return base + 4;        /* residue 4 */
        case 2: return base + 5;        /* residue 5 */
        case 3: return base + 6;        /* residue 6 */
        default: return base;           /* unreachable */
    }
}
