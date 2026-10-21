/*
 * A075782  Numbers n such that perfect_power(n)+perfect_power(n+1) is prime.
 * https://oeis.org/A075782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth integer m (m ≥ 1) such that * perfect_power(m) +
 *            perfect_power(m+1) is prime, * where perfect_power(k) is the
 *            k‑th perfect power in increasing order. * (See OEIS A075782.)
 * Range:     u8 n <= 57, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Miller–Rabin primality test for 64‑bit numbers ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* trial division by small primes */
    static const uint64_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d * 2^s  with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1u) == 0u) {
        d >>= 1;
        ++s;
    }

    /* bases that are deterministic for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, power = a, exp = d;
        while (exp) {
            if (exp & 1) x = (x * power) % n;
            power = (power * power) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- comparison function for qsort (uint64_t) ---------- */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t u = *(const uint64_t *)a;
    uint64_t v = *(const uint64_t *)b;
    if (u < v) return -1;
    if (u > v) return 1;
    return 0;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* static data, initialized once */
    static bool init = false;
    static uint64_t *pp = NULL;       /* array of perfect powers */
    static size_t pp_len = 0;         /* number of perfect powers stored */

    if (!init) {
        /* generate perfect powers up to a fixed numeric limit */
        const uint64_t LIMIT = 1000000000ULL;   /* 1e9 is sufficient for n ≤ 10000 */
        size_t cap = 50000;                    /* initial capacity */
        pp = (uint64_t *)malloc(cap * sizeof(uint64_t));
        size_t idx = 0;

        /* 1 is the first perfect power (2^0 considered) */
        pp[idx++] = 1;

        for (uint64_t base = 2; base * base <= LIMIT; ++base) {
            __uint128_t val = (__uint128_t)base * base;
            while (val <= LIMIT) {
                if (idx >= cap) {
                    cap *= 2;
                    pp = (uint64_t *)realloc(pp, cap * sizeof(uint64_t));
                }
                pp[idx++] = (uint64_t)val;
                val *= base;
            }
        }
        pp_len = idx;

        /* sort and deduplicate */
        qsort(pp, pp_len, sizeof(uint64_t), cmp_uint64);
        size_t newlen = 0;
        for (size_t i = 0; i < pp_len; ++i) {
            if (i == 0 || pp[i] != pp[i-1]) {
                pp[newlen++] = pp[i];
            }
        }
        pp_len = newlen;

        init = true;
    }

    /* search for the n‑th qualifying integer */
    size_t found = 0;
    for (size_t m = 1; m + 1 <= pp_len; ++m) {   /* m is 1‑based index */
        uint64_t sum = pp[m-1] + pp[m];          /* pp[m] is (m+1)-th perfect power */
        if (is_prime_uint64(sum)) {
            ++found;
            if (found == (size_t)n) {
                return (seq_t)m;
            }
        }
    }

    /* if not found (should not happen for given bounds) */
    return 0;
}
