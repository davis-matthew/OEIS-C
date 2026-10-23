/*
 * A038934  Primes p such that 55 is not a square mod p.
 * https://oeis.org/A038934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes p such that 55 is not a quadratic residue modulo p */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) using 128‑bit arithmetic */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = ((uint64_t)1 << 32);   /* sqrt(2^64‑1) < 2^32 */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* Modular exponentiation: (base^exp) mod mod */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

/* Test primality of n using the primes already found (simple trial division) */
static bool is_prime_uint64(uint64_t n, const uint64_t *prime_list, size_t prime_cnt)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    uint64_t lim = isqrt_uint64(n);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= lim; ++i)
        if (n % prime_list[i] == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* static state for prime generation */
    static uint64_t lower_limit = 3;          /* next candidate to test for primality */
    static uint64_t *prime_list = NULL;      /* all primes found */
    static size_t prime_cap        = 0;
    static size_t prime_cnt        = 0;

    /* static state for sequence cache */
    static seq_t *seq_buf = NULL;            /* cached sequence values */
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;

    /* initialise buffers on first call */
    if (seq_buf == NULL) {
        seq_cap = 128;
        seq_buf = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }
    if (prime_list == NULL) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    /* generate values until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* find next prime */
        uint64_t p = lower_limit;
        for (;;) {
            if (p & 1) {                     /* skip even numbers except 2 (skipped) */
                if (is_prime_uint64(p, prime_list, prime_cnt))
                    break;
            }
            ++p;
        }

        /* store prime for future divisibility tests */
        if (prime_cnt == prime_cap) {
            prime_cap *= 2;
            prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_cnt++] = p;
        lower_limit = p + 1;                 /* start next search above this prime */

        /* Legendre symbol (55|p) via Euler's criterion
           (55|p) = 1 if 55 is a quadratic residue,
                    p-1 ≡ -1 if 55 is a non‑residue,
                    0 if p divides 55.  */
        uint64_t leg = mod_pow_uint64(55ULL, (p - 1) / 2, p);
        if (leg == p - 1) {                   /* p > 2 guarantees correct test */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_buf = (seq_t *)realloc(seq_buf, seq_cap * sizeof(seq_t));
            }
            seq_buf[seq_len++] = (seq_t)p;
        }
    }

    return seq_buf[(size_t)n - 1];
}
