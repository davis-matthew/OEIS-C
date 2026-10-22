/*
 * A004712  Positions of ones in binary expansion of e-2.
 * https://oeis.org/A004712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 1 bit in the binary expansion of e-2
 *            = Sum_{k>=2} 1/k!; the bits come from an exact mixed-radix
 *            (factorial-base) spigot, recomputed at higher precision when
 *            more are needed
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A004712_pos = NULL;
static idx_t A004712_npos = 0;

static void A004712_compute(uint64_t B) {
    /* choose N with log2(N!) >= B + 64, using floor(log2 k) as a lower bound */
    uint64_t need = B + 64, lg = 0, N = 2;
    while (lg < need) {
        N++;
        uint64_t t = N, f = 0;
        while (t > 1) { t >>= 1; f++; }
        lg += f;
    }
    unsigned int *d = (unsigned int *)malloc((size_t)(N + 1) * sizeof(unsigned int));
    uint64_t k, p;
    for (k = 0; k <= N; k++) d[k] = 1;
    uint64_t *pos = (uint64_t *)malloc((size_t)(B + 1) * sizeof(uint64_t));
    idx_t cnt = 0;
    for (p = 1; p <= B; p++) {
        unsigned int carry = 0;
        for (k = N; k >= 2; k--) {
            unsigned int x = d[k] * 2u + carry;
            d[k] = x % (unsigned int)k;
            carry = x / (unsigned int)k;
        }
        if (carry) pos[cnt++] = p;
    }
    free(d);
    free(A004712_pos);
    A004712_pos = pos;
    A004712_npos = cnt;
}

seq_t a(idx_t n) {
    uint64_t B = 256;
    while (A004712_npos < n) {
        while (B < 3 * (uint64_t)n + 64) B *= 2;
        A004712_compute(B);
        B *= 2;
    }
    return (seq_t)A004712_pos[n - 1];
}
