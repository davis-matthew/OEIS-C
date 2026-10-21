/*
 * A069207  Numbers n such that n^2 + (reversal of n)^2 is prime. Ignore
 *          leading 0's.
 * https://oeis.org/A069207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069207:
   a(n) = the n‑th integer m (m ≥ 1) such that m^2 + rev(m)^2 is prime.
   rev(m) is the decimal reversal of m (ignoring leading zeros). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------------------------------------------------------- */
/* Utility: reverse the decimal digits of a 64‑bit unsigned int */
static uint64_t rev_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x != 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* ----------------------------------------------------------- */
/* Deterministic Miller–Rabin primality test for 64‑bit integers */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((unsigned __int128)a * b) % mod);
}

static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;

    /* Small prime trial division */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* Write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ----------------------------------------------------------- */
/* Memoised storage for the sequence values */
static seq_t *cache      = NULL;   /* array of computed terms                */
static idx_t  cache_len  = 0;      /* number of terms stored so far          */
static size_t cache_cap  = 0;      /* capacity of the cache array            */
static uint64_t next_test = 1;     /* next integer to check for primality    */

/* Ensure that the cache can store at least 'need' elements. */
static void grow_cache(idx_t need)
{
    if (cache_cap >= (size_t)need) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failed – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* ----------------------------------------------------------- */
/* Public function: return the n‑th term of A069207 (a(1)=1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 – return 0 */

    /* Generate terms until the requested index is available. */
    while (cache_len < n) {
        uint64_t m       = next_test;
        uint64_t rev_m   = rev_uint64(m);
        unsigned __int128 sum = (unsigned __int128)m * m +
                                (unsigned __int128)rev_m * rev_m;

        bool prime = false;
        if (sum <= UINT64_MAX) {
            prime = isPrime64((uint64_t)sum);
        } else {
            /* For completeness, but this branch never occurs for the
               expected range of n. */
            /* If a generic arbitrary‑precision primality test were
               available you could call it here. */
        }

        if (prime) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)m;   /* store the found term       */
        }
        ++next_test;
    }

    return cache[n - 1];           /* a(1) is stored at cache[0] */
}
