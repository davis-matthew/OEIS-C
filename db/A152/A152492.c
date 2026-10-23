/*
 * A152492  a(n) = number of integers of the form (n*k)^2/(k^2 - n^2).
 * https://oeis.org/A152492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A152492: a(n) = number of integers of the form
    (n*k)^2 / (k^2 - n^2)   (k integer, k != n, 1 <= k <= n^2 + 1)   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* helpers ---------------------------------------------------------- */

static uint64_t int_isqrt(uint64_t x)
{
    /* integer square root (floor) for 64‑bit values */
    uint64_t r = 0, h = 1ULL << 32;
    while (h != 0) {
        if (r + h <= x / (r + h))
            r += h;
        h >>= 1;
    }
    return r;
}

/* generate all divisors of n^4 ------------------------------------------------*/

static void gen_divisors(const uint64_t *primes, const int *exps4,
                         int exps_cnt, int idx, uint64_t cur,
                         uint64_t *buf, int *cnt)
{
    if (idx == exps_cnt) {
        buf[(*cnt)++] = cur;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t val = 1;
    for (int e = 0; e <= exps4[idx]; ++e) {
        gen_divisors(primes, exps4, exps_cnt, idx + 1, cur * val,
                     buf, cnt);
        val *= p;
    }
}

/* main function ---------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* factor n -------------------------------------------------- */
    uint64_t primes[64];
    int    exps[64];
    int    pcnt = 0;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* exponents of n^4 ------------------------------------------- */
    int exps4[64];
    for (int i = 0; i < pcnt; ++i)
        exps4[i] = 4 * exps[i];

    /* generate all divisors of n^4 -------------------------------- */
    /* upper bound on divisor count: product (4e+1) <= 20000 for n<=1e4 */
    uint64_t divbuf[20000];
    int divcnt = 0;
    gen_divisors(primes, exps4, pcnt, 0, 1, divbuf, &divcnt);

    uint64_t n2 = (uint64_t)n * (uint64_t)n;

    uint64_t cnt = 0;
    for (int i = 0; i < divcnt; ++i) {
        uint64_t dpos = divbuf[i];

        /* positive divisor d = +dpos : k > n */
        uint64_t v = n2 + dpos;
        uint64_t s = int_isqrt(v);
        if (s * s == v)
            ++cnt;

        /* negative divisor d = -dpos : k < n, only if n^2 - dpos > 0 */
        if (n2 > dpos) {
            v = n2 - dpos;
            s = int_isqrt(v);
            if (s * s == v)
                ++cnt;
        }
    }

    return (seq_t)cnt;
}
