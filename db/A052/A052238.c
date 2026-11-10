/*
 * A052238  Primes p from A031924 such that A052180(p) = 23.
 * https://oeis.org/A052238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th prime p such that p+6 is the next prime (p+2
 *            and p+4 composite) and max(lpf(p+2), lpf(p+4)) = 23
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A052238_res = NULL;
static size_t A052238_cnt = 0, A052238_cap = 0;
static uint64_t A052238_next = 25;

static uint64_t A052238_mulmod(uint64_t x, uint64_t y, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)x * y) % m);
}

static uint64_t A052238_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A052238_mulmod(r, b, m);
        b = A052238_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

/* deterministic Miller-Rabin for 64-bit */
static bool A052238_isprime(uint64_t n)
{
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = A052238_powmod(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = A052238_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* smallest prime factor <= 23 of odd x, or 0 if none */
static unsigned A052238_small(uint64_t x)
{
    static const unsigned sp[] = {3, 5, 7, 11, 13, 17, 19, 23};
    for (int i = 0; i < 8; i++)
        if (x % sp[i] == 0)
            return sp[i];
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A052238_cnt < (size_t)n) {
        uint64_t p = A052238_next;
        A052238_next += 2;
        unsigned u = A052238_small(p + 2);
        if (!u) continue;
        unsigned v = A052238_small(p + 4);
        if (!v) continue;
        if ((u > v ? u : v) != 23) continue;
        if (!A052238_isprime(p) || !A052238_isprime(p + 6)) continue;
        if (A052238_cnt == A052238_cap) {
            A052238_cap = A052238_cap ? A052238_cap * 2 : 256;
            uint64_t *tmp = (uint64_t *)realloc((void *)A052238_res,
                                                A052238_cap * sizeof(uint64_t));
            if (!tmp) abort();
            A052238_res = tmp;
        }
        A052238_res[A052238_cnt++] = p;
    }
    return (seq_t)A052238_res[n - 1];
}
