/*
 * A004593  Expansion of e in base 2.
 * https://oeis.org/A004593
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons, base
 * Formula:   a(n) = floor(e*2^(n-3)) mod 2; binary digits of e =
 *            10.1011011111..., with the fractional bits produced by the
 *            mixed-radix spigot e - 2 = Sum_{k>=2} 1/k! (digits all 1 in base
 *            1/2,1/3,1/4,...)
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *e_bits = NULL;   /* e_bits[i] = (i+1)-th fractional bit */
static idx_t e_nbits = 0;

static void e_compute(idx_t nbits) {
    /* choose K with log2(K!) > nbits + 64 */
    double lg = 0.0;
    idx_t K = 1;
    while (lg <= (double)nbits + 64.0) {
        K++;
        /* log2(K) without math.h: crude but monotone-safe bound via integer bits */
        {
            idx_t t = K; int b = 0;
            while (t > 1) { t >>= 1; b++; }
            lg += (double)b;          /* floor(log2 K) <= log2 K, so K is safely large */
        }
    }
    uint32_t *d = (uint32_t *)malloc((size_t)(K + 1) * sizeof(uint32_t));
    idx_t k;
    for (k = 0; k <= K; k++) d[k] = 1;
    unsigned char *bits = (unsigned char *)malloc((size_t)nbits);
    idx_t i;
    for (i = 0; i < nbits; i++) {
        uint64_t carry = 0;
        for (k = K; k >= 2; k--) {
            uint64_t x = (uint64_t)d[k] * 2 + carry;
            d[k] = (uint32_t)(x % k);
            carry = x / k;
        }
        bits[i] = (unsigned char)carry;
    }
    free(d);
    free(e_bits);
    e_bits = bits;
    e_nbits = nbits;
}

seq_t a(idx_t n) {
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)0;
    idx_t idx = n - 4;
    if (idx >= e_nbits) {
        idx_t want = e_nbits ? e_nbits : 1024;
        while (want <= idx) want *= 2;
        e_compute(want);
    }
    return (seq_t)e_bits[idx];
}
