/*
 * A004227  a(n) = ceiling(1000*log_10(n)).
 * https://oeis.org/A004227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(1000*log_10(n)); computed exactly as the number
 *            of decimal digits D of n^1000 (exact bignum), minus 1 if n is a
 *            power of 10
 * Range:     u8 n <= 1, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A004227_BASE 1000000000ULL

seq_t a(idx_t n) {
    uint64_t nl[3];
    int nlen = 0;
    uint64_t t = (uint64_t)n;
    while (t > 0) { nl[nlen++] = t % A004227_BASE; t /= A004227_BASE; }

    size_t cap = (size_t)1000 * (size_t)nlen + 4;
    uint32_t *r = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(cap, sizeof(uint32_t));
    size_t rlen = 1;
    r[0] = 1;

    for (int step = 0; step < 1000; step++) {
        size_t tlen = rlen + (size_t)nlen + 1;
        size_t i;
        for (i = 0; i < tlen; i++) tmp[i] = 0;
        for (i = 0; i < rlen; i++) {
            uint64_t carry = 0;
            int j;
            for (j = 0; j < nlen; j++) {
                uint64_t cur = (uint64_t)tmp[i + j] + (uint64_t)r[i] * nl[j] + carry;
                tmp[i + j] = (uint32_t)(cur % A004227_BASE);
                carry = cur / A004227_BASE;
            }
            size_t k = i + (size_t)nlen;
            while (carry) {
                uint64_t cur = (uint64_t)tmp[k] + carry;
                tmp[k] = (uint32_t)(cur % A004227_BASE);
                carry = cur / A004227_BASE;
                k++;
            }
        }
        while (tlen > 1 && tmp[tlen - 1] == 0) tlen--;
        uint32_t *sw = r; r = tmp; tmp = sw;
        rlen = tlen;
    }

    uint64_t top = r[rlen - 1];
    uint64_t D = 9 * (uint64_t)(rlen - 1);
    while (top > 0) { D++; top /= 10; }
    free(r);
    free(tmp);

    /* n power of 10? */
    uint64_t m = (uint64_t)n;
    while (m % 10 == 0) m /= 10;
    uint64_t res = (m == 1) ? D - 1 : D;

    seq_t out = 0;
    out += res;
    return out;
}
