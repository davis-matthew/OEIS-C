/*
 * A004257  a(n) = round(log_2(n)).
 * https://oeis.org/A004257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_2(sqrt(2)*n)), i.e. k = floor(log_2(n)), plus 1
 *            if n^2 >= 2^(2k+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    unsigned int k = 0;
    idx_t temp = n;
    while (temp > 1) {
        temp >>= 1;
        k++;
    }
    unsigned __int128 nsq = (unsigned __int128)n * (unsigned __int128)n;
    unsigned __int128 thr = (unsigned __int128)1 << (2 * k + 1);
    idx_t r = (idx_t)k + (nsq >= thr ? 1 : 0);
    return (seq_t)r;
}
