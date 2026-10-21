/*
 * A073086  Floor[concatenation of eight consecutive numbers from n to n+7
 *          divided by 8].
 * https://oeis.org/A073086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor(concatenation of the eight consecutive integers *
 *            starting with n, divided by 8)
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 0; i < 8; ++i) {
        seq_t val = (seq_t)(n + i);

        /* Compute digit count of val */
        idx_t digit_count = 0;
        seq_t temp = val;
        if (temp == 0) {
            digit_count = 1;
        } else {
            while (temp != 0) {
                temp /= 10;
                digit_count++;
            }
        }

        /* Compute 10^digit_count */
        seq_t mult = 1;
        for (idx_t k = 0; k < digit_count; ++k) {
            mult *= 10;
        }

        /* Append the digits of val to the concatenated number */
        res = res * mult + val;
    }

    return res / 8;   /* floor division by 8 */
}
