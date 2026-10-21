/*
 * A071259  Integer part of the arithmetic mean of the n-th prime p(n) and the
 *          n-th composite number C(n).
 * https://oeis.org/A071259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((p(n) + C(n)) / 2) where p(n) is the nth prime and
 *            C(n) the nth composite
 * Range:     u8 n <= 54, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation lazily generates primes and composites on demand.
   The first 50000 primes and composites are stored in static buffers
   so that subsequent calls are O(1).  For the ranges used in the tests
   (n up to about 10 000) this is easily fast enough. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define MAXCAP 50000    /* enough for all test indices */

/* ----------------- Miller–Rabin (64‑bit) ----------------- */
static uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((unsigned __int128)a * b % mod);
}

static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    base %= mod;
    while (exp)
    {
        if (exp & 1) res = modmul64(res, base, mod);
        base = modmul64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r)
        {
            x = modmul64(x, x, n);
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ----------------- Buffers for primes and composites ----------------- */
static uint64_t prime_buf[MAXCAP];
static uint64_t comp_buf[MAXCAP];
static idx_t prime_cnt = 0;
static idx_t comp_cnt  = 0;
static uint64_t next_check = 2;   /* next integer to test */

/* ----------------- Sequence function ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset behaviour irrelevant */

    /* Keep generating until we have at least n primes and n composites. */
    while (prime_cnt < n || comp_cnt < n)
    {
        if (is_prime_uint64(next_check))
        {
            if (prime_cnt < MAXCAP) prime_buf[prime_cnt++] = next_check;
        }
        else if (next_check >= 4)   /* first composite is 4 */
        {
            if (comp_cnt < MAXCAP) comp_buf[comp_cnt++] = next_check;
        }
        ++next_check;
    }

    seq_t p = (seq_t)prime_buf[n-1];
    seq_t c = (seq_t)comp_buf[n-1];
    return (p + c) / (seq_t)2;   /* floor of the arithmetic mean */
}
