/*
 * A104377  Number of distinct prime divisors of numbers derived from three
 *          consecutive concatenated odd numbers.
 * https://oeis.org/A104377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct prime divisors of the integer formed
 *            by concatenating the three consecutive odd numbers
 *            (2n‑1),(2n+1),(2n+3) in decimal
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_uint64.h"

/* ---------- small helpers ---------- */

/* modular multiplication (a*b) % m using 128‑bit arithmetic */
static uint64_t mulmod_uint64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

/* modular exponentiation */
static uint64_t powmod_uint64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1ULL) r = mulmod_uint64(r, a, m);
        a = mulmod_uint64(a, a, m);
        e >>= 1ULL;
    }
    return r;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0ULL) return false;
    }
    /* 64‑bit deterministic bases */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1ULL; ++s; }
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod_uint64(a, d, n);
        if (x == 1ULL || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod_uint64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Pollard Rho factorization for 64‑bit integers */
static uint64_t pollard_rho_uint64(uint64_t n)
{
    if ((n & 1ULL) == 0ULL) return 2ULL;
    static const uint64_t c_vals[] = {1ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t ci = 0; ci < sizeof(c_vals)/sizeof(c_vals[0]); ++ci) {
        uint64_t c = c_vals[ci];
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1ULL) {
            x = (mulmod_uint64(x, x, n) + c) % n;
            y = (mulmod_uint64(y, y, n) + c) % n;
            y = (mulmod_uint64(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_uint64(diff, n);
        }
        if (d > 1ULL && d < n) return d;
    }
    /* fallback (should not happen for 64‑bit inputs) */
    return n;
}

/* count distinct prime divisors of n (n <= 1e15 < 2^54) */
static uint64_t count_distinct_prime_factors_uint64(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t factors[64];
    int cnt = 0;
    /* recursive factorial using explicit stack to avoid recursion depth limit */
    struct stack_item { uint64_t v; };
    struct stack_item stack[64];
    int sp = 0;
    stack[sp++] = (struct stack_item){n};
    while (sp > 0) {
        uint64_t v = stack[--sp].v;
        if (v == 1) continue;
        if (is_prime_uint64(v)) {
            factors[cnt++] = v;
            continue;
        }
        uint64_t d = pollard_rho_uint64(v);
        stack[sp++] = (struct stack_item){d};
        stack[sp++] = (struct stack_item){v / d};
    }
    /* sort factors and count distinct */
    if (cnt == 0) return 0;
    /* bubble sort (cnt <= 64) */
    for (int i = 0; i < cnt - 1; ++i)
        for (int j = 0; j < cnt - 1 - i; ++j)
            if (factors[j] > factors[j+1]) {
                uint64_t t = factors[j];
                factors[j] = factors[j+1];
                factors[j+1] = t;
            }
    uint64_t distinct = 1;
    for (int i = 1; i < cnt; ++i)
        if (factors[i] != factors[i-1]) ++distinct;
    return distinct;
}

/* number of decimal digits of x */
static unsigned int digits_uint64(uint64_t x)
{
    unsigned int d = 0;
    do { ++d; x /= 10ULL; } while (x);
    return d;
}

/* 10^exp for exp <= 20 */
static uint64_t pow10_uint64(unsigned int exp)
{
    uint64_t r = 1;
    while (exp--) r *= 10ULL;
    return r;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1, undefined for n=0 */
    uint64_t a1 = 2ULL * (uint64_t)n - 1ULL;        /* 2n-1 */
    uint64_t b1 = 2ULL * (uint64_t)n + 1ULL;        /* 2n+1 */
    uint64_t c1 = 2ULL * (uint64_t)n + 3ULL;        /* 2n+3 */

    unsigned int db = digits_uint64(b1);
    unsigned int dc = digits_uint64(c1);

    uint64_t x = a1 * pow10_uint64(db + dc) + b1 * pow10_uint64(dc) + c1;

    uint64_t cnt = count_distinct_prime_factors_uint64(x);

    return (seq_t)cnt;
}
