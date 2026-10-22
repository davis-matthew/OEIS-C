/*
 * A004270  a(n) = ceiling(10000*log_2(n)).
 * https://oeis.org/A004270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(10000*log_2(n)); log_2(n) is computed exactly in
 *            integer fixed point by the binary repeated-squaring method (56
 *            fraction bits)
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    unsigned e = 0;
    while ((m >> e) > 1) e++;               /* e = floor(log2 n) */

    /* x = n / 2^e in [1,2), stored with 63 fraction bits */
    uint64_t x = m << (63 - e);
    uint64_t f = 0;                          /* fractional bits of log2 n */
    int i;
    for (i = 0; i < 56; i++) {
        unsigned __int128 p = (unsigned __int128)x * x;   /* value p / 2^126 in [1,4) */
        f <<= 1;
        if (p >> 127) {
            f |= 1;
            x = (uint64_t)(p >> 64);
        } else {
            x = (uint64_t)(p >> 63);
        }
    }

    unsigned __int128 q = (unsigned __int128)f * 10000u;
    uint64_t ip = (uint64_t)(q >> 56);
    uint64_t rem = (uint64_t)(q & (((unsigned __int128)1 << 56) - 1));
    uint64_t res = (uint64_t)e * 10000u + ip + (rem != 0 ? 1 : 0);

    seq_t r = 0;
    r += res;
    return r;
}
