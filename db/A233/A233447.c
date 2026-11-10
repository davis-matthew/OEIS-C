/*
 * A233447  Floor(compositorial(n) / n!), that is, floor(A036691(n) /
 *          A000142(n)).
 * https://oeis.org/A233447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(A036691(n) / A000142(n)); the product of the first
 *            n composites is built as a big integer, then floor-divided by 2,
 *            3, ..., n in turn (floor(floor(x/a)/b) = floor(x/(ab)))
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    size_t cap = (size_t)(2 * n + 8);
    uint32_t *d = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t len = 1, k;
    d[0] = 1;
    idx_t count = 0, i = 4;
    while (count < n) {
        if (!is_prime(i)) {
            uint64_t carry = 0;
            for (k = 0; k < len; k++) {
                uint64_t t = (uint64_t)d[k] * (uint64_t)i + carry;
                d[k] = (uint32_t)t;
                carry = t >> 32;
            }
            if (carry) d[len++] = (uint32_t)carry;
            count++;
        }
        i++;
    }
    for (idx_t m = 2; m <= n; m++) {
        uint64_t rem = 0;
        for (k = len; k-- > 0;) {
            uint64_t cur = (rem << 32) | d[k];
            d[k] = (uint32_t)(cur / m);
            rem = cur % m;
        }
        while (len > 1 && d[len - 1] == 0) len--;
    }
    seq_t r = 0;
    for (k = len; k-- > 0;) {
        r = (r << 16) << 16;
        r = r + (seq_t)d[k];
    }
    free(d);
    return r;
}
