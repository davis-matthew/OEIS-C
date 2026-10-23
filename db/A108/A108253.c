/*
 * A108253  Numbers k such that little googol + k is prime.
 * https://oeis.org/A108253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A108253: Numbers k such that 2^100 + k is prime.
   The function returns the k at index n (n >= 1). */
/* Implementation uses a simple sieve-less prime generator based on
   Miller–Rabin with 128‑bit arithmetic. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 uint128;
static const uint128 GOOGOL = ((uint128)1) << 100;   /* little googol = 2^100 */

/* Small base set for deterministic Miller–Rabin up to 2^128
   (probabilistic, but enough for this range). */
static const uint32_t mr_bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};
static const size_t NUM_BASIS = sizeof(mr_bases) / sizeof(mr_bases[0]);

/* Multiply a and b modulo mod using only 128‑bit arithmetic
   without overflow.  All numbers are < mod (< 2^128). */
static uint128 mulmod128(uint128 a, uint128 b, uint128 mod)
{
    uint128 result = 0;
    a %= mod;
    while (b) {
        if (b & 1) {
            result += a;
            if (result >= mod) result -= mod;
        }
        b >>= 1;
        if (b) {
            a <<= 1;
            if (a >= mod) a -= mod;
        }
    }
    return result;
}

/* Compute a^e mod mod. */
static uint128 powmod128(uint128 a, uint128 e, uint128 mod)
{
    uint128 result = 1;
    a %= mod;
    while (e) {
        if (e & 1) result = mulmod128(result, a, mod);
        e >>= 1;
        if (e) a = mulmod128(a, a, mod);
    }
    return result;
}

/* Miller–Rabin primality test for 128‑bit integers. */
static bool isPrime128(uint128 n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,
                                            13ULL,17ULL,19ULL,23ULL,29ULL,
                                            31ULL,37ULL,41ULL,43ULL,47ULL,
                                            53ULL,59ULL,61ULL,67ULL,71ULL,
                                            73ULL,79ULL,83ULL,89ULL,97ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    uint128 d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    for (size_t i = 0; i < NUM_BASIS; ++i) {
        uint128 a = mr_bases[i] % n;
        if (a == 0) continue;
        uint128 x = powmod128(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mulmod128(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Dynamic array to store found k values. */
static seq_t *values = NULL;
static size_t cap = 0;
static size_t len = 0;
static uint128 next_candidate = 0;     /* next odd number to test */

/* Ensure capacity for at least n elements. */
static void ensure_cap(size_t n)
{
    if (n <= cap) return;
    size_t new_cap = cap ? cap * 2 : 1024;
    while (new_cap < n) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    values = new_arr;
    cap = new_cap;
}

/* Generate primes until we have at least n terms. */
static void generate_terms(idx_t n)
{
    if (next_candidate == 0) {
        /* Initialize for first call */
        next_candidate = GOOGOL + 1;
        if ((next_candidate & 1) == 0) ++next_candidate;   /* ensure odd */
    }

    while ((idx_t)len < n) {
        if (isPrime128(next_candidate)) {
            ensure_cap(len + 1);
            values[len++] = (seq_t)(next_candidate - GOOGOL);
        }
        next_candidate += 2;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined; offset is 1 */
    generate_terms(n);
    return values[n - 1];
}
