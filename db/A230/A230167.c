/*
 * A230167  The number of multinomial coefficients over partitions with value
 *          equal to 6.
 * https://oeis.org/A230167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1/12)*(n-3)^2)+floor((n-1)*(1/5))+
 *            ((1+(-1)^n)*(1/2))*floor((n-2)*(1/4))
 * Range:     u8 n <= 18, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t diff, sq, term1, term2, term3;

    /* Compute (n-3)^2 safely for n < 3 */
    diff = (n >= 3) ? (n - 3) : (3 - n);
    sq = diff * diff;
    term1 = sq / 12;          /* floor((n-3)^2 / 12) */

    term2 = (n - 1) / 5;      /* floor((n-1)/5) */

    /* The third term is present only when n is even */
    if ((n & 1) == 0) {
        term3 = (n >= 2) ? ((n - 2) / 4) : 0; /* floor((n-2)/4) */
    } else {
        term3 = 0;
    }

    return term1 + term2 + term3;
}
