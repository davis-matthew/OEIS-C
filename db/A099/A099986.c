/*
 * A099986  Bisection of A001113 (digits of e).
 * https://oeis.org/A099986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A001113(2n+1), the (2n+1)-th decimal digit of e (e =
 *            Sum_{k>=0} 1/k!), computed by a mixed-radix spigot
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *A099986_digits = NULL;
static idx_t A099986_have = 0;

static void A099986_compute(idx_t D)
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
    free(A099986_digits);
    A099986_digits = d;
    A099986_have = D;
}

static unsigned char A099986_e_digit(idx_t m)
{
    if (m < 1) return 0;
    if (m > A099986_have) {
        idx_t D = m + 32;
        idx_t g = A099986_have + A099986_have / 2;
        if (g > D) D = g;
        A099986_compute(D);
    }
    return A099986_digits[m - 1];
}

seq_t a(idx_t n)
{
    return (seq_t)A099986_e_digit(2 * n + 1);
}
