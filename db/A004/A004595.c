/*
 * A004595  Expansion of e in base 4.
 * https://oeis.org/A004595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(1) = 2; for n >= 2, a(n) = the (n-1)-th base-4 digit after the
 *            point of e - 2 = Sum_{k>=2} 1/k!. The digits come from a
 *            mixed-radix spigot (multiply by 4 repeatedly)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *e4_digits = NULL; /* e4_digits[i] = a(i+1) */
static idx_t e4_len = 0;

static void e4_compute(idx_t want)
{
    idx_t L = want + 16;            /* guard digits */
    idx_t N = 2;
    uint64_t bits = 0;
    uint64_t need = 2 * (uint64_t)L + 64;
    unsigned int *d;
    idx_t i, j;

    /* choose N with N! > 4^L (floor(log2 k) is a lower bound for log2 k) */
    while (bits < need) {
        idx_t k = N + 1;
        uint64_t b = 0;
        while ((k >> (b + 1)) != 0) b++;
        bits += b;
        N++;
    }

    d = (unsigned int *)malloc((size_t)(N + 1) * sizeof(unsigned int));
    for (j = 0; j <= N; j++) d[j] = 1;   /* e - 2 = 0.1111... in mixed radix 1/2,1/3,... */

    free(e4_digits);
    e4_digits = (unsigned char *)malloc((size_t)L);
    e4_digits[0] = 2;
    for (i = 1; i < L; i++) {
        unsigned int carry = 0;
        for (j = N; j >= 2; j--) {
            unsigned int x = d[j] * 4u + carry;
            d[j] = x % (unsigned int)j;
            carry = x / (unsigned int)j;
        }
        e4_digits[i] = (unsigned char)carry;
    }
    free(d);
    e4_len = want;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > e4_len) {
        idx_t want = e4_len ? e4_len : 1024;
        while (want < n) want *= 2;
        e4_compute(want);
    }
    return (seq_t)e4_digits[n - 1];
}
