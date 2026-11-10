/*
 * A060173  Number of orbits of length n under a map whose periodic points are
 *          counted by A056045.
 * https://oeis.org/A060173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} mu(d) * A056045(n/d), where A056045(m)
 *            = Sum_{e|m} binomial(m,e). Big intermediate values are held in a
 *            small base-2^16 limb bignum, so the result is exact whenever
 *            a(n) fits in seq_t
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 41, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* SHARED_HELPER: Moebius function mu(n) by trial division */
static int A060173_mu(idx_t n)
{
    int r = 1;
    for (idx_t p = 2; p <= n / p; ++p)
    {
        if (n % p == 0)
        {
            n /= p;
            if (n % p == 0)
                return 0;
            r = -r;
        }
    }
    if (n > 1)
        r = -r;
    return r;
}

/* little-endian limbs, base 65536, stored in uint32_t */
static void A060173_mul(uint32_t *x, size_t *len, uint64_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i)
    {
        uint64_t t = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(t & 0xFFFFu);
        carry = t >> 16;
    }
    while (carry)
    {
        x[(*len)++] = (uint32_t)(carry & 0xFFFFu);
        carry >>= 16;
    }
}

static void A060173_div(uint32_t *x, size_t *len, uint64_t m)
{
    uint64_t rem = 0;
    for (size_t i = *len; i-- > 0;)
    {
        uint64_t cur = (rem << 16) | x[i];
        x[i] = (uint32_t)(cur / m);
        rem = cur % m;
    }
    while (*len > 0 && x[*len - 1] == 0)
        --(*len);
}

static void A060173_add(uint32_t *acc, size_t *alen, const uint32_t *x, size_t xlen)
{
    uint32_t carry = 0;
    size_t i = 0;
    for (; i < xlen || carry; ++i)
    {
        uint32_t t = carry + (i < *alen ? acc[i] : 0) + (i < xlen ? x[i] : 0);
        acc[i] = t & 0xFFFFu;
        carry = t >> 16;
    }
    if (i > *alen)
        *alen = i;
}

/* acc -= x, requires acc >= x */
static void A060173_sub(uint32_t *acc, size_t *alen, const uint32_t *x, size_t xlen)
{
    int64_t borrow = 0;
    for (size_t i = 0; i < *alen; ++i)
    {
        int64_t t = (int64_t)acc[i] - borrow - (int64_t)(i < xlen ? x[i] : 0);
        if (t < 0)
        {
            t += 65536;
            borrow = 1;
        }
        else
            borrow = 0;
        acc[i] = (uint32_t)t;
    }
    while (*alen > 0 && acc[*alen - 1] == 0)
        --(*alen);
}

/* tmp := binomial(m, k) */
static void A060173_binom(uint32_t *tmp, size_t *len, idx_t m, idx_t k)
{
    if (k > m - k)
        k = m - k;
    tmp[0] = 1;
    *len = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        A060173_mul(tmp, len, (uint64_t)(m - k + i));
        A060173_div(tmp, len, (uint64_t)i);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    size_t cap = (size_t)(n / 16) + 16;
    uint32_t *pos = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *neg = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(cap, sizeof(uint32_t));
    size_t plen = 0, nlen = 0, tlen = 0;

    for (idx_t d = 1; d <= n; ++d)
    {
        if (n % d != 0)
            continue;
        int mu = A060173_mu(d);
        if (mu == 0)
            continue;
        idx_t m = n / d;
        for (idx_t e = 1; e <= m; ++e)
        {
            if (m % e != 0)
                continue;
            A060173_binom(tmp, &tlen, m, e);
            if (mu > 0)
                A060173_add(pos, &plen, tmp, tlen);
            else
                A060173_add(neg, &nlen, tmp, tlen);
        }
    }
    A060173_sub(pos, &plen, neg, nlen);
    A060173_div(pos, &plen, (uint64_t)n);

    seq_t r = 0;
    for (size_t i = plen; i-- > 0;)
    {
        r = r * 65536;
        r += (seq_t)pos[i];
    }
    free(pos);
    free(neg);
    free(tmp);
    return r;
}
