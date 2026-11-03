/*
 * A000396  Perfect numbers k: k is equal to the sum of the proper divisors of
 *          k.
 * https://oeis.org/A000396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, core, changed
 * Formula:   a(n) = 2^(p-1)*(2^p - 1) where p = A000043(n) is the n-th
 *            Mersenne prime exponent, found by searching primes p with trial
 *            factoring and the Lucas-Lehmer test (assumes there are no odd
 *            perfect numbers)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   is_prime_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u32.h"

/* Lucas-Lehmer test for 2^p - 1, p an odd prime */
static int pn_lucas_lehmer(unsigned p)
{
    size_t L = (p + 31) / 32, i, j;
    unsigned bits = p - 32 * (unsigned)(L - 1);
    uint32_t mask = bits == 32 ? 0xffffffffu : ((1u << bits) - 1);
    unsigned w = p / 32, b = p % 32;
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(2 * L + 1, sizeof(uint32_t));
    unsigned it;
    int res;

    s[0] = 4;
    for (it = 0; it + 2 < p; it++) {
        uint64_t carry;
        uint32_t c;
        memset(r, 0, (2 * L + 1) * sizeof(uint32_t));
        for (i = 0; i < L; i++) {
            carry = 0;
            if (s[i] == 0) continue;
            for (j = 0; j < L; j++) {
                uint64_t t = (uint64_t)r[i + j] + (uint64_t)s[i] * s[j] + carry;
                r[i + j] = (uint32_t)t;
                carry = t >> 32;
            }
            r[i + L] = (uint32_t)carry;
        }
        /* s = lo + hi, lo = r mod 2^p, hi = r >> p */
        carry = 0;
        for (i = 0; i < L; i++) {
            uint32_t lo = r[i];
            uint32_t hi;
            if (i == L - 1) lo &= mask;
            hi = r[w + i] >> b;
            if (b) hi |= r[w + i + 1] << (32 - b);
            if (i == L - 1) hi &= mask;
            carry += (uint64_t)lo + hi;
            s[i] = (uint32_t)carry;
            carry >>= 32;
        }
        c = s[L - 1] >> bits;
        s[L - 1] &= mask;
        for (i = 0; c && i < L; i++) {
            uint64_t t = (uint64_t)s[i] + c;
            s[i] = (uint32_t)t;
            c = (uint32_t)(t >> 32);
        }
        /* s -= 2 mod M */
        {
            int ge2 = s[0] >= 2;
            for (i = 1; !ge2 && i < L; i++)
                if (s[i]) ge2 = 1;
            if (ge2) {
                uint32_t bor = 2;
                for (i = 0; bor && i < L; i++) {
                    uint32_t old = s[i];
                    s[i] = old - bor;
                    bor = old < bor ? 1 : 0;
                }
            } else {
                uint32_t v = s[0];
                for (i = 0; i < L; i++) s[i] = 0xffffffffu;
                s[L - 1] = mask;
                s[0] -= (2 - v);
            }
        }
    }
    {
        int zero = 1, full = 1;
        for (i = 0; i < L; i++) {
            if (s[i]) zero = 0;
            if (s[i] != (i == L - 1 ? mask : 0xffffffffu)) full = 0;
        }
        res = zero || full;
    }
    free(s);
    free(r);
    return res;
}

static uint64_t pn_powmod2(unsigned e, uint64_t q)
{
    uint64_t r = 1 % q, b = 2 % q;
    while (e) {
        if (e & 1) r = r * b % q;
        b = b * b % q;
        e >>= 1;
    }
    return r;
}

/* returns 1 if 2^p-1 has a proper factor q = 2kp+1 found by trial */
static int pn_small_factor(unsigned p)
{
    uint64_t k;
    for (k = 1; k <= 4000; k++) {
        uint64_t q = 2 * k * (uint64_t)p + 1;
        if (q >= ((uint64_t)1 << 32)) break;
        if (p < 63 && q >= (((uint64_t)1 << p) - 1)) break;
        if (q % 8 != 1 && q % 8 != 7) continue;
        if (pn_powmod2(p, q) == 1) return 1;
    }
    return 0;
}

static int pn_width(void)
{
    seq_t x = 1;
    int k = 0;
    while (k < 300) {
        x <<= 1;
        k++;
        if (x == 0) return k;
    }
    return 0; /* arbitrary precision */
}

seq_t a(idx_t n)
{
    static unsigned ex[256];
    static idx_t cnt = 0;
    static unsigned next_p = 2;
    static int width = -1;
    unsigned p;
    seq_t h, m;

    if (n < 1 || n > 256) return 0;
    if (width < 0) width = pn_width();

    while (cnt < n) {
        p = next_p;
        if (width > 0 && 2 * (uint64_t)p - 1 > (uint64_t)width) return 0;
        next_p++;
        if (!is_prime_u32(p)) continue;
        if (p == 2 || (!pn_small_factor(p) && pn_lucas_lehmer(p)))
            ex[cnt++] = p;
    }

    p = ex[n - 1];
    if (width > 0 && 2 * (uint64_t)p - 1 > (uint64_t)width) return 0;
    m = 1;
    m <<= p;
    m -= 1;
    h = 1;
    h <<= (p - 1);
    return h * m;
}
