/*
 * A088267  Smallest number that on prefixing with the n-th term of A088265
 *          yields prime.
 * https://oeis.org/A088267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest p >= 1 such that the concatenation
 *            p||A088265(n+1) is prime, where A088265(m) = 10^((m-1)/4+1) +
 *            {1,3,7,9}[m mod 4]; found by search with Miller-Rabin
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 85 (2026-11-07)
 * Helpers:   mulmod_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mulmod_u128.h"

static const unsigned A088267_bases[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};

static unsigned __int128 A088267_powmod128(unsigned __int128 b, unsigned __int128 e, unsigned __int128 m)
{
    unsigned __int128 r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = mulmod_u128(r, b, m);
        b = mulmod_u128(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A088267_isprime128(unsigned __int128 n)
{
    if (n < 2) return false;
    for (int i = 0; i < 20; i++) {
        unsigned __int128 p = A088267_bases[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    unsigned __int128 d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 20; i++) {
        unsigned __int128 x = A088267_powmod128(A088267_bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = mulmod_u128(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static seq_t A088267_powmod_seq(seq_t b, seq_t e, seq_t m)
{
    seq_t r = 1;
    b = b % m;
    while (e > 0) {
        if (e % 2 == 1) r = (r * b) % m;
        b = (b * b) % m;
        e = e / 2;
    }
    return r;
}

static bool A088267_isprime_seq(seq_t n)
{
    if (n < 2) return false;
    for (int i = 0; i < 20; i++) {
        seq_t p = (seq_t)A088267_bases[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    seq_t d = n - 1;
    int s = 0;
    while (d % 2 == 0) { d = d / 2; s++; }
    for (int i = 0; i < 20; i++) {
        seq_t x = A088267_powmod_seq((seq_t)A088267_bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t m = n + 1;                 /* index into A088265 (offset 1) */
    idx_t k = (m - 1) / 4 + 1;
    idx_t r = m % 4;
    unsigned d;
    if (r == 1) d = 1;
    else if (r == 2) d = 3;
    else if (r == 3) d = 7;
    else d = 9;

    unsigned long long p = 1;

    /* Fast path in unsigned __int128 while the candidate fits. */
    if (k + 1 <= 37) {
        unsigned __int128 pk = 1;
        for (idx_t i = 0; i < k; i++) pk *= 10;
        unsigned __int128 B = pk + d;
        unsigned __int128 shift = pk * 10;      /* B has k+1 digits */
        unsigned __int128 umax = ~(unsigned __int128)0;
        unsigned __int128 plim = (umax - B) / shift;
        for (; (unsigned __int128)p <= plim; p++) {
            if (A088267_isprime128((unsigned __int128)p * shift + B))
                return (seq_t)p;
        }
    }

    /* General path in seq_t (arbitrary-precision builds). */
    seq_t pk = 1;
    for (idx_t i = 0; i < k; i++) pk *= 10;
    seq_t B = pk + (seq_t)d;
    seq_t shift = pk * 10;
    for (seq_t q = (seq_t)p;; q++) {
        if (A088267_isprime_seq(q * shift + B)) return q;
    }
}
