/*
 * A106747  Replace each odd digit d of n with (d-1)/2 and each even digit d
 *          with d/2.
 * https://oeis.org/A106747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Interpret the decimal digits of n after mapping each *
 *            digit d by * if d is even: d/2 * if d is odd : (d-1)/2 * and
 *            then treat the resulting digit sequence as a decimal number
 * Range:     u8 n <= 599, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n == 0 */
    if (n == 0) {
        return 0;
    }

    /* Mapping table for decimal digits 0–9 */
    static const unsigned char map[10] = {0,0,1,1,2,2,3,3,4,4};

    /* Store digits in reverse order */
    idx_t digits[20];
    idx_t len = 0;
    while (n > 0) {
        digits[len++] = n % 10;
        n /= 10;
    }

    /* Build the resulting number by processing digits from most
     * significant to least significant. */
    seq_t result = 0;
    for (idx_t i = len; i > 0; --i) {
        unsigned char mapped = map[digits[i-1]];
        result = result * 10 + (seq_t)mapped;
    }

    return result;
}
