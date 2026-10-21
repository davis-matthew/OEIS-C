/*
 * A050786  Palindromic primes containing at least one pair of consecutive
 *          equal digits.
 * https://oeis.org/A050786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   has_consecutive_equal, mod_mul_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050786: palindromic primes that contain at least one pair of
   consecutive equal digits. The function generates palindromic numbers
   in increasing order, tests them for primality with a deterministic
   Miller‑Rabin algorithm (sufficient for all 64‑bit values) and keeps
   the terms in a static cache. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_consecutive_equal.h"
#include "../utils/mod_mul_seq.h"

/* ---------- small primes used for trial division ---------- */
static const uint64_t SMALL_PRIMES[] =
    {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

/* ---------- Miller‑Rabin bases for 64‑bit integers ---------- */
static const uint64_t MR_BASES[] =
    {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

/* ---------- fast power of 10 for uint64_t (used for half ranges) ---------- */
static uint64_t pow10_uint64(unsigned exp)
{
    uint64_t r = 1;
    while (exp--) r *= 10ULL;
    return r;
}

/* ---------- build a palindrome from its half ---------- */
static seq_t build_pal(uint64_t half, bool odd)
{
    seq_t res = (seq_t)half;
    uint64_t tmp = odd ? half / 10ULL : half;   /* skip middle digit when odd */
    while (tmp > 0)
    {
        res = res * 10 + (tmp % 10);
        tmp /= 10;
    }
    return res;
}

/* ---------- modular exponentiation ---------- */
static seq_t mod_pow_seq(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result = mod_mul_seq(result, base, mod);
        base = mod_mul_seq(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* ---------- deterministic Miller‑Rabin primality test for seq_t ---------- */
static bool isPrime(seq_t n)
{
    if (n < (seq_t)2) return false;

    /* trial division by small primes */
    for (size_t i = 0; i < sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]); ++i)
    {
        uint64_t p = SMALL_PRIMES[i];
        if (n % (seq_t)p == 0)
            return n == (seq_t)p;
    }

    /* express n‑1 as d·2^s with d odd */
    seq_t d = n - (seq_t)1;
    unsigned s = 0;
    while ((d & (seq_t)1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* test the fixed bases */
    for (size_t i = 0; i < sizeof(MR_BASES)/sizeof(MR_BASES[0]); ++i)
    {
        uint64_t a_uint = MR_BASES[i];
        seq_t a = (seq_t)a_uint;
        if ((a % n) == 0) continue;          /* a ≡ 0 mod n */

        seq_t x = mod_pow_seq(a, d, n);
        if (x == (seq_t)1 || x == n - (seq_t)1) continue;

        bool passed = false;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mod_mul_seq(x, x, n);
            if (x == n - (seq_t)1)
            {
                passed = true;
                break;
            }
        }
        if (!passed) return false;
    }
    return true;
}

/* ---------- cache and generator state ---------- */
static seq_t *cache = NULL;          /* cache of sequence terms; cache[i] = a(i+1) */
static size_t cache_len = 0, cache_cap = 0;

/* generation parameters for palindromic lengths */
static size_t gen_len = 0;      /* current palindrome length */
static uint64_t gen_half_start = 0;
static uint64_t gen_half_end   = 0;
static uint64_t gen_half_curr  = 0;
static bool gen_started = false;

/* generate palindromic primes until at least target terms are cached */
static void generate_up_to(size_t target)
{
    if (cache_len >= target) return;

    /* grow cache array if needed */
    if (cache_cap < target)
    {
        cache_cap = target * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* initialise generator on first call */
    if (!gen_started)
    {
        gen_len = 2; /* smallest length that can contain a repeated digit pair */
        unsigned half_len = (gen_len % 2 ? (gen_len + 1) / 2 : gen_len / 2);
        gen_half_start = (half_len == 1) ? 1ULL : pow10_uint64(half_len - 1);
        gen_half_end   = pow10_uint64(half_len) - 1;
        gen_half_curr  = gen_half_start;
        gen_started = true;
    }

    /* main loop: generate terms until we have enough */
    while (cache_len < target)
    {
        seq_t candidate = build_pal(gen_half_curr, gen_len % 2 == 1);

        if (has_consecutive_equal(candidate) && isPrime(candidate))
            cache[cache_len++] = candidate;

        /* move to next half; if reached the end, increase length */
        if (gen_half_curr == gen_half_end)
        {
            ++gen_len;
            unsigned half_len = (gen_len % 2 ? (gen_len + 1) / 2 : gen_len / 2);
            gen_half_start = (half_len == 1) ? 1ULL : pow10_uint64(half_len - 1);
            gen_half_end   = pow10_uint64(half_len) - 1;
            gen_half_curr  = gen_half_start;
        }
        else
        {
            ++gen_half_curr;
        }
    }
}

/* ---------- required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined; return 0 for consistency */
    generate_up_to((size_t)n);
    return cache[n - 1];
}
