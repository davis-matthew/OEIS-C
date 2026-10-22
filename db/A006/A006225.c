/*
 * A006225  Rabbytes: group eight successive Fibonacci numbers in reversed
 *          binary and translate to decimal.
 * https://oeis.org/A006225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number whose binary expansion is the concatenation of the
 *            reversed binary expansions of F(max(0,n-7)), ..., F(n), where F
 *            = A000045 (F(0) contributes a single "0")
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   build_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_bits.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    size_t W = (size_t)(n / 32 + 2);
    uint32_t *fp = (uint32_t *)calloc(W, sizeof(uint32_t)); /* F(k-1) */
    uint32_t *fc = (uint32_t *)calloc(W, sizeof(uint32_t)); /* F(k) */
    fc[0] = 1;
    size_t cap = 1024, len = 0;
    unsigned char *bits = (unsigned char *)malloc(cap);
    idx_t k;
    for (k = 1; k <= n; k++) {
        size_t i;
        if (k + 7 >= n) {
            /* bit length of F(k) */
            size_t top = W;
            while (top > 0 && fc[top - 1] == 0) top--;
            size_t nb = 0;
            if (top > 0) {
                uint32_t t = fc[top - 1];
                nb = (top - 1) * 32;
                while (t) { nb++; t >>= 1; }
            }
            if (len + nb > cap) {
                while (len + nb > cap) cap *= 2;
                bits = (unsigned char *)realloc(bits, cap);
            }
            /* reversed binary: least significant bit first */
            for (i = 0; i < nb; i++)
                bits[len++] = (unsigned char)((fc[i / 32] >> (i % 32)) & 1u);
        }
        /* advance: (fp, fc) <- (fc, fp + fc) */
        uint64_t carry = 0;
        for (i = 0; i < W; i++) {
            uint64_t s = (uint64_t)fp[i] + fc[i] + carry;
            fp[i] = fc[i];
            fc[i] = (uint32_t)s;
            carry = s >> 32;
        }
    }
    seq_t res = len ? build_bits(bits, 0, len) : (seq_t)0;
    free(bits);
    free(fp);
    free(fc);
    return res;
}
