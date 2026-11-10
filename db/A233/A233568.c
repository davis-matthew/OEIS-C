/*
 * A233568  a(n) is number in A114994 which c-equivalent to c-factorial of n
 *          (A047778).
 * https://oeis.org/A233568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number obtained by writing the binary parts (1
 *            followed by k>=0 zeros) of the concatenation of 1..n
 *            (A047778(n)) in nonincreasing order of length (the A114994
 *            representative); computed by counting parts per zero-run length
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t cnt[66];
    int i;
    for (i = 0; i < 66; i++) cnt[i] = 0;
    int maxk = 0;
    for (idx_t m = 1; m <= n; m++) {
        int zeros = 0;
        int started = 0;
        int top = 63;
        while (top > 0 && !((m >> top) & 1)) top--;
        for (int b = top; b >= 0; b--) {
            if ((m >> b) & 1) {
                if (started) {
                    cnt[zeros]++;
                    if (zeros > maxk) maxk = zeros;
                }
                started = 1;
                zeros = 0;
            } else {
                zeros++;
            }
        }
        if (started) {
            cnt[zeros]++;
            if (zeros > maxk) maxk = zeros;
        }
    }
    seq_t res = 0;
    for (int k = maxk; k >= 0; k--) {
        seq_t part = (seq_t)1 << k;
        for (uint64_t c = 0; c < cnt[k]; c++) {
            res = (res << (k + 1)) | part;
        }
    }
    return res;
}
