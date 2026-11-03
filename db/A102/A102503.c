/*
 * A102503  Primes of the concatenated form 9nn9.
 * https://oeis.org/A102503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the nth prime of the form 9nn9 (concatenation of 9, n, n,
 *            9)
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 2500, u128 n <= 2500,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 2224 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* local helper: count decimal digits of an unsigned integer */
static inline int count_digits(idx_t x) {
    int c = 0;
    do {
        ++c;
        x /= 10;
    } while (x);
    return c;
}

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

/* ---- Miller–Rabin helpers for 64‑bit integers ---- */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1ULL)
            r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool miller_rabin(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2ULL == 0)  return n == 2;
    if (n % 3ULL == 0)  return n == 3;

    /* write n-1 = d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;          /* base divisible by n: pass */

        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---- trial division by small primes ---- */
static bool is_prime(uint64_t n)
{
    static const uint32_t small_primes[] =
    {
        2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U, 23U, 29U,
        31U, 37U, 41U, 43U, 47U, 53U, 59U, 61U, 67U, 71U
    };

    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    return miller_rabin(n);
}

/* Miller–Rabin primality test for arbitrary-precision values. */
static bool isprime_big(seq_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    idx_t r = 0;
    while ((d & 1) == 0) {
        d = d / 2;
        r = r + 1;
    }

    /* Bases sufficient for a probabilistic test. */
    const uint64_t bases[] = {2,3,5,7,11,13,17,19,23,29};
    for (idx_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        seq_t a = (seq_t)bases[i];
        if (a >= n) break;

        /* modular exponentiation x = a^d mod n */
        seq_t x = 1;
        seq_t base = a % n;
        seq_t exp  = d;
        while (exp > 0) {
            if ((exp & 1) != 0) x = (x * base) % n;
            base = (base * base) % n;
            exp  = exp >> 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool cont = false;
        for (idx_t j = 1; j < r; ++j) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t target)
{
    if (target == 0)
        return 0;

    idx_t count = 0;      /* number of primes found so far   */
    idx_t n = 10;         /* the n in the concatenation; n>=10 */

    while (1) {
        /* number of decimal digits of n */
        int d = count_digits(n);

        /* 10^d and 10^(d+1) */
        seq_t pow10_d  = pow10(d);       /* 10^d          */
        seq_t pow10_d1 = pow10(d + 1);   /* 10^(d+1)      */

        /* term = 9*10^(2d+1) + n*(10^(d+1)+10) + 9
           = 9*(pow10_d1 * pow10_d) + n*(pow10_d1 + 10) + 9   */
        seq_t term = 9 * (pow10_d1 * pow10_d) +
                     (seq_t)n * (pow10_d1 + 10) + 9;

        /* primality test : small numbers use the fast 64‑bit routine */
        bool prime;
        if (term <= (seq_t)UINT64_MAX) {
            prime = is_prime((uint64_t)term);
        } else {
            prime = isprime_big(term);
        }

        if (prime) {
            ++count;
            if (count == target)
                return term;
        }
        ++n;
    }
}
