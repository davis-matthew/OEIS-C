/*
 * A169886  Fourth powers (n * n * n * n) in carryless arithmetic mod 10.
 * https://oeis.org/A169886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n^4 in carryless arithmetic modulo 10 (digitwise addition
 *            mod 10)
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Convert n to digits (least‑significant first) */
    int d[128] = {0};
    size_t dn = 0;
    idx_t temp = n;
    if (temp == 0) {
        d[0] = 0;
        dn = 1;
    } else {
        while (temp > 0) {
            d[dn++] = (int)(temp % 10);
            temp /= 10;
        }
    }

    /* Compute n^2 (carryless convolution) */
    int sq[256] = {0};
    size_t sq_len = 2 * dn - 1;
    for (size_t i = 0; i < dn; ++i) {
        for (size_t j = 0; j < dn; ++j) {
            int prod = d[i] * d[j];
            sq[i + j] = (sq[i + j] + prod) % 10;
        }
    }

    /* Compute n^4 by convolving n^2 with itself */
    int sq4[512] = {0};
    size_t sq4_len = 2 * sq_len - 1; /* = 4*dn - 3 */
    for (size_t i = 0; i < sq_len; ++i) {
        for (size_t j = 0; j < sq_len; ++j) {
            int prod = sq[i] * sq[j];
            sq4[i + j] = (sq4[i + j] + prod) % 10;
        }
    }

    /* Skip leading zeros when converting back to decimal string */
    size_t pos = sq4_len;
    do {
        --pos;
    } while (pos > 0 && sq4[pos] == 0);

    char buf[512];
    size_t idx = 0;
    for (size_t k = pos; ; --k) {
        buf[idx++] = '0' + sq4[k];
        if (k == 0) break;
    }
    buf[idx] = '\0';

    return seq_from_str(buf);
}
