/*
 * A001113  Decimal expansion of e.
 * https://oeis.org/A001113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, nice, core
 * Formula:   e = 2 + (1 + (1 + (1 + ...)/4)/3)/2, i.e. e = Sum_{k>=0} 1/k!;
 *            a(n) is the n-th decimal digit, computed with the mixed-radix
 *            spigot (Sale's algorithm): repeatedly multiply the fractional
 *            part, held in factorial base, by 10 and take the integer carry
 *            as the next digit
 * Range:     u8 n <= 7954, u16 n <= 7954, u32 n <= 7954, u64 n <= 7954,
 *            u128 n <= 7954, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *e_digits = NULL;
static idx_t e_have = 0;

static void e_compute(idx_t D)
{
    idx_t total = D + 40;
    uint64_t s = 0, N = 1;
    while (s * 30103ULL <= (uint64_t)(total + 10) * 100000ULL) {
        uint64_t k, lg;
        N++;
        k = N; lg = 0;
        while (k > 1) { k >>= 1; lg++; }
        s += lg;
    }
    uint32_t *r = (uint32_t *)malloc((size_t)(N + 1) * sizeof(uint32_t));
    unsigned char *d = (unsigned char *)malloc((size_t)(total + 1));
    for (uint64_t i = 0; i <= N; i++) r[i] = 1;
    d[0] = 2;
    for (idx_t j = 1; j < total; j++) {
        uint64_t carry = 0;
        for (uint64_t i = N; i >= 2; i--) {
            uint64_t x = (uint64_t)r[i] * 10ULL + carry;
            r[i] = (uint32_t)(x % i);
            carry = x / i;
        }
        d[j] = (unsigned char)carry;
    }
    free(r);
    free(e_digits);
    e_digits = d;
    e_have = D;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > e_have) {
        idx_t D = n + 32;
        idx_t g = e_have + e_have / 2;
        if (g > D) D = g;
        e_compute(D);
    }
    return (seq_t)e_digits[n - 1];
}
