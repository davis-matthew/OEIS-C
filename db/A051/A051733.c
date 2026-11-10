/*
 * A051733  Numbers n such that A051732(n) = n-1.
 * https://oeis.org/A051733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (p+1)/2 where p runs through the primes with primitive
 *            root 2 (A001122); found by a search, cached
 * Range:     u8 n <= 39, u16 n <= 4591, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Validated: standard, pass, 75 terms through n = 5916 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t A051733_mulmod(uint64_t x, uint64_t y, uint64_t m)
{
    return (uint64_t)((unsigned __int128)x * y % m);
}

static uint64_t A051733_pow2mod(uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m, b = 2 % m;
    while (e) {
        if (e & 1) r = A051733_mulmod(r, b, m);
        b = A051733_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A051733_isprime(uint64_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

/* true iff p = 2k-1 is an odd prime and 2 is a primitive root mod p */
static bool A051733_good(uint64_t k)
{
    if (k < 2) return false;
    uint64_t p = 2 * k - 1;
    if (!A051733_isprime(p)) return false;
    uint64_t pm1 = p - 1;
    uint64_t t = pm1;
    for (uint64_t q = 2; q * q <= t; q++) {
        if (t % q == 0) {
            while (t % q == 0) t /= q;
            if (A051733_pow2mod(pm1 / q, p) == 1) return false;
        }
    }
    if (t > 1) {
        if (A051733_pow2mod(pm1 / t, p) == 1) return false;
    }
    return true;
}

static uint64_t *A051733_terms = NULL;
static size_t A051733_cnt = 0, A051733_cap = 0;
static uint64_t A051733_next = 2;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t want = (size_t)n;
    while (A051733_cnt < want) {
        if (A051733_good(A051733_next)) {
            if (A051733_cnt == A051733_cap) {
                size_t nc = A051733_cap ? A051733_cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(A051733_terms, nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                A051733_terms = p;
                A051733_cap = nc;
            }
            A051733_terms[A051733_cnt++] = A051733_next;
        }
        A051733_next++;
    }
    return (seq_t)A051733_terms[want - 1];
}
