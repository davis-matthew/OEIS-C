/*
 * A024781  Every suffix prime and no 0 digits in base 6 (written in base 6).
 * https://oeis.org/A024781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   A024781 – numbers written in base‑6 with digits 1–5 only such
 *            that every suffix (in base‑6) is prime. a(1) is the first term
 * Range:     u8 none, u16 none, u32 n <= 212, u64 n <= 454, u128 n <= 454,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Deterministic Miller–Rabin primality test for 64‑bit integers.        */
/* Uses a known set of bases that is correct for all 64‑bit numbers.     */
/* --------------------------------------------------------------------- */

static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod)
{
#if defined(__SIZEOF_INT128__)
    return (uint64_t)(((__uint128_t)a * b) % mod);
#else
    uint64_t result = 0;
    while (b) {
        if (b & 1) result = (result + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return result;
#endif
}

static uint64_t powmod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e) {
        if (e & 1) res = mulmod(res, a, mod);
        a = mulmod(a, a, mod);
        e >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small prime checks */
    static const uint64_t small_primes[] = {
        2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull,
        19ull, 23ull, 29ull, 31ull, 37ull
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic set of bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ull, 325ull, 9375ull, 28178ull,
        450775ull, 9780504ull, 1795265022ull
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -------------------------------------------------------------- */
/* Cache for the sequence (maximum 500 numbers – sequence has 454). */
/* -------------------------------------------------------------- */

static seq_t   cache[500];
static size_t  cache_len = 0;
static bool    built = false;

/* DFS that builds numbers by prepending digits 1‑5.                */
static void dfs(seq_t decVal, seq_t baseVal,
                idx_t len, seq_t pow10, seq_t pow6)
{
    if (cache_len < 500) cache[cache_len++] = decVal;

    for (uint8_t d = 1; d <= 5; ++d) {
        seq_t newDec  = (seq_t)d * pow10 + decVal;
        seq_t newBase = (seq_t)d * pow6  + baseVal;
        if (is_prime_uint64((uint64_t)newBase))
            dfs(newDec, newBase, len + 1,
                pow10 * (seq_t)10, pow6 * (seq_t)6);
    }
}

/* Comparison function for qsort. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* Build the entire sequence once and sort it. */
static void build_sequence(void)
{
    if (built) return;
    built = true;

    /* Start with the one‑digit primes 2, 3, 5. */
    uint8_t primes[] = {2, 3, 5};
    for (size_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i)
        dfs((seq_t)primes[i], (seq_t)primes[i], 1,
            (seq_t)10, (seq_t)6);

    qsort(cache, cache_len, sizeof(seq_t), cmp_seq);
}

/* ------------------------------------------------------------------ */
/* Public function required by the build.                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset is 1 */
    build_sequence();
    if ((idx_t)n > (idx_t)cache_len) return (seq_t)0;
    return cache[n - 1];
}
