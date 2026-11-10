/*
 * A075552  Primes generated in A073628.
 * https://oeis.org/A075552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A073628(n-1) + A073628(n) + A073628(n+1), where
 *            A073628(0)=0, A073628(1)=1, A073628(2)=2, and A073628(k) =
 *            smallest m > A073628(k-1) such that A073628(k-2)+A073628(k-1)+m
 *            is prime (found by search)
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool A075552_is_prime(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof witnesses / sizeof witnesses[0]; ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, power = a;
        uint64_t r = d;
        while (r) {
            if (r & 1) x = (x * power) % n;
            power = (power * power) % n;
            r >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t j = 1; j < s; ++j) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

static seq_t *A075552_cache = NULL;
static idx_t A075552_size = 0;
static idx_t A075552_cap = 0;

static void A075552_ensure(idx_t need)
{
    if (A075552_cap >= need) return;
    idx_t new_cap = (A075552_cap > 0) ? (A075552_cap * 3) / 2 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = mem_resize(seq_t, A075552_cache, new_cap);
    A075552_cache = tmp;
    A075552_cap = new_cap;
}

static void A075552_compute(idx_t n)
{
    if (n < A075552_size) return;
    A075552_ensure(n + 3);
    if (A075552_size == 0) {
        A075552_cache[0] = (seq_t)0;
        A075552_cache[1] = (seq_t)1;
        A075552_cache[2] = (seq_t)2;
        A075552_size = 3;
    }
    for (idx_t i = A075552_size; i <= n; ++i) {
        uint64_t p2 = (uint64_t)A075552_cache[i - 2];
        uint64_t p1 = (uint64_t)A075552_cache[i - 1];
        uint64_t cand = p1 + 1;
        while (!A075552_is_prime(p2 + p1 + cand)) ++cand;
        A075552_cache[i] = (seq_t)cand;
    }
    A075552_size = n + 1;
}

seq_t a(idx_t n)
{
    A075552_compute(n + 1);
    seq_t s = A075552_cache[n - 1];
    s += A075552_cache[n];
    s += A075552_cache[n + 1];
    return s;
}
