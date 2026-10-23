/*
 * A129123  Number of 4-tuples of standard tableau with height less than or
 *          equal to 2.
 * https://oeis.org/A129123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..floor(n/2)} ((n-2*j+1)/(n-j+1))^4 *
 *            binomial(n,j)^4
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t comb = 1;          /* binomial(n,0) */
    idx_t half = n / 2;
    for (idx_t j = 0; j <= half; ++j) {
        /* compute the rational factor (n-2j+1)/(n-j+1) */
        idx_t num = n - 2*j + 1;
        idx_t den = n - j + 1;
        seq_t t = (comb * num) / den;   /* always integer */
        seq_t t2 = t * t;
        seq_t t4 = t2 * t2;
        sum += t4;

        /* update binomial to next value for j+1 */
        if (j < half) {
            comb = (comb * (n - j)) / (j + 1);
        }
    }
    return sum;
}
