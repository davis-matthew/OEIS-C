/*
 * A112854  Primes of the form (2*n)!/n!-1.
 * https://oeis.org/A112854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime( (2*m)!/m! - 1 ) with offset 1 (i.e. a(n) is the
 *            nth prime among the numbers (2*m)!/m! - 1, where m runs through
 *            1,2,3,... )
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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

static inline bool term_is_prime(seq_t x) {
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        return is_prime((uint64_t)x);
    } else {
        return isprime_big(x);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* unused, offset 1 */
    seq_t P = (seq_t)2;            /* P_1 = product_{i=1}^1 (1+i) = 2 */
    idx_t m = 1;                   /* current m (i.e., n in the definition) */
    idx_t count = 0;

    while (true) {
        seq_t term = P - 1;        /* current candidate */
        if (term_is_prime(term)) {
            ++count;
            if (count == n) return term;
        }

        /* advance to next m: compute P_{m+1} from P_m */
        ++m;                                   /* now m = old m + 1 */
        seq_t divisor = (seq_t)(m);
        seq_t factor1 = (seq_t)(2 * (m - 1) + 1); /* 2*(prev m)+1 */
        seq_t factor2 = (seq_t)(2 * (m - 1) + 2); /* 2*(prev m)+2 */
        P = (P / divisor) * factor1 * factor2;
    }
}
