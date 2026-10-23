/*
 * A106776  Primes with digit sum = 44.
 * https://oeis.org/A106776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3850, u64 n <= 3850, u128 n <= 3850,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the nth prime whose decimal digit sum equals 44.  
   Indexing follows OEIS offset: a(1) = 98999.  The implementation runs in
   O(n·√p) time by incremental trial division using deterministic
   Miller–Rabin for 64‑bit numbers.  All data are stored in a static
   array so that subsequent calls are fast. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- helpers for 64‑bit arithmetic ---------- */
static inline uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1ULL;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) result = modmul64(result, base, mod);
        base = modmul64(base, base, mod);
        exp >>= 1U;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
                                    31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0ULL) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1U; ++s; }
    /* bases for deterministic test for 64‑bit integers */
    static const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = modmul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Sum of decimal digits of a 64‑bit integer. */
static uint32_t digit_sum_uint64(uint64_t x)
{
    uint32_t sum = 0U;
    while (x) {
        sum += (uint32_t)(x % 10ULL);
        x /= 10ULL;
    }
    return sum;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */

    /* static state for incremental generation */
    static uint64_t *primes = NULL;
    static size_t cap = 0;
    static size_t size = 0;
    static uint64_t next = 2ULL;

    /* generate until we have at least n primes with digit sum 44 */
    while (size < n) {
        uint64_t p = next;
        /* advance next: skip even numbers after 2 */
        if (p <= 2ULL) next = 3ULL;
        else next += 2ULL;

        /* primality test */
        if (isPrime64(p) && digit_sum_uint64(p) == 44U) {
            /* grow array if necessary */
            if (size == cap) {
                size_t newcap = cap ? cap << 1 : 16U;
                uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
                if (!tmp) return 0;          /* allocation failure */
                primes = tmp;
                cap = newcap;
            }
            primes[size++] = p;
        }
    }

    return (seq_t)primes[n - 1U];
}
