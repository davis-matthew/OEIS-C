/*
 * A004225  a(n) = 1000*log_10(n) rounded down.
 * https://oeis.org/A004225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1000*log_10(n)) = (number of decimal digits of
 *            n^1000) - 1, computed exactly by forming n^1000 as a base-10^9
 *            big integer
 * Range:     u8 n <= 1, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static uint32_t d[2400];
    const uint32_t B = 1000000000u;
    int L = 1, i, j;
    long long digits;
    uint32_t top;

    d[0] = 1;
    for (i = 0; i < 1000; i++) {
        unsigned __int128 c = 0;
        for (j = 0; j < L; j++) {
            c += (unsigned __int128)d[j] * n;
            d[j] = (uint32_t)(c % B);
            c /= B;
        }
        while (c != 0) {
            d[L++] = (uint32_t)(c % B);
            c /= B;
        }
    }

    top = d[L - 1];
    digits = 9LL * (L - 1);
    while (top > 0) {
        digits++;
        top /= 10;
    }
    return (seq_t)(digits - 1);
}
