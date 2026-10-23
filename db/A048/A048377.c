/*
 * A048377  Append d digits d after each digit d in decimal expansion of n.
 * https://oeis.org/A048377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, nice, look
 * Formula:   a(n) = concat over decimal digits d of d repeated (d+1) times
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 98, u128 n <= 8998,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Extract decimal digits of n in reverse order */
    uint8_t digits[20];
    int dcnt = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        digits[dcnt++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    seq_t res = 0;
    for (int idx = dcnt - 1; idx >= 0; --idx) {
        int d = digits[idx];
        int k = d + 1;           /* repetitions of this digit */

        /* Compute block: d repeated k times */
        seq_t block = 0;
        for (int j = 0; j < k; ++j)
            block = block * 10 + d;

        /* Compute 10^k */
        seq_t pow10k = 1;
        for (int j = 0; j < k; ++j)
            pow10k *= 10;

        res = res * pow10k + block;
    }

    return res;
}
