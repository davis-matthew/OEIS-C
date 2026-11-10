/*
 * A064822  Number of indices 1 <= i <= n with A001113(i)=A001113(n).
 * https://oeis.org/A064822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = #{ 1 <= i <= n : A001113(i) = A001113(n) }, where
 *            A001113(i) is the i-th decimal digit of e (spigot, then count by
 *            direct scan)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Validated: partial, pass, 96 terms through n = 96 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *A001113_e_digits = NULL;
static idx_t A001113_e_have = 0;

static void A001113_e_compute(idx_t D)
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
    free(A001113_e_digits);
    A001113_e_digits = d;
    A001113_e_have = D;
}

static void A001113_ensure(idx_t n)
{
    if (n > A001113_e_have) {
        idx_t D = n + 32;
        idx_t g = A001113_e_have + A001113_e_have / 2;
        if (g > D) D = g;
        A001113_e_compute(D);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    A001113_ensure(n);
    unsigned char t = A001113_e_digits[n - 1];
    unsigned long long cnt = 0;
    for (idx_t i = 0; i < n; i++)
        if (A001113_e_digits[i] == t) cnt++;
    return (seq_t)cnt;
}
