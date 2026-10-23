/*
 * A104696  Rearrangement of positive integers: change odd digits d to 10-d.
 * https://oeis.org/A104696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = transform n by replacing every odd decimal digit d with
 *            10 - d (i.e., 1→9,3→7,5→5,7→3,9→1), keeping even digits
 *            unchanged. The digits remain in their original positions
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is index (uint64_t).  For any n >= 1, compute the mapped value. */
    uint64_t temp = (uint64_t)n;
    seq_t res = 0;

    /* Store the digits of n from least significant to most (reversed). */
    unsigned int digits[20];   /* 2^64-1 has at most 20 decimal digits */
    int sz = 0;
    do {
        digits[sz++] = temp % 10;
        temp /= 10;
    } while (temp);

    /* Process digits from most significant to least to build the result. */
    for (int i = sz - 1; i >= 0; --i) {
        unsigned int d = digits[i];
        unsigned int md = (d & 1) ? (10 - d) : d;   /* mapped digit */
        res = res * 10 + (seq_t)md;
    }
    return res;
}
