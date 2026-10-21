/*
 * A086981  a(n) = smallest k where (10^k+1)=0 mod prime(n)^2, or 0 if no such
 *          k exists.
 * https://oeis.org/A086981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k where (10^k+1)=0 mod prime(n)^2, or 0 if no
 *            such k exists
 * Range:     u8 n <= 9, u16 n <= 72, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIEVE_LIMIT      200000
#define MAX_PRIME_COUNT  15000

/* ---------- sieve for the first primes ---------- */
static uint32_t primes_list[MAX_PRIME_COUNT];
static size_t   prime_count = 0;
static bool     sieve_done  = false;

static void init_primes (void)
{
    if (sieve_done) return;
    bool *marked = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!marked) return;
    memset(marked, 0, (SIEVE_LIMIT + 1) * sizeof(bool));
    for (uint32_t i = 2; i <= SIEVE_LIMIT && prime_count < MAX_PRIME_COUNT; ++i) {
        if (!marked[i]) {
            primes_list[prime_count++] = i;
            if (i * i <= SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    marked[j] = true;
            }
        }
    }
    free(marked);
    sieve_done = true;
}

/* ---------- fast modular exponentiation for uint64_t ---------- */
static uint64_t pow_mod_uint64 (uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    init_primes();
    if ((size_t)(n - 1) >= prime_count) return 0; /* safety */
    uint32_t p = primes_list[n - 1];
    if (p == 2 || p == 5) return 0;               /* 10 and –1 are not powers mod 4 or 25 */

    uint64_t p64  = (uint64_t)p;
    uint64_t mod64 = p64 * p64;                   /* p^2 */
    uint64_t order = p64 * (p64 - 1);             /* φ(p^2) */

    /* gather distinct prime factors of order – i.e. p and the primes of p-1 */
    uint64_t factors[64];
    size_t   fact_cnt = 0;
    uint64_t temp = p64 - 1;
    for (size_t i = 0; i < prime_count && (uint64_t)primes_list[i] * primes_list[i] <= temp; ++i) {
        uint32_t q = primes_list[i];
        if (temp % q == 0) {
            factors[fact_cnt++] = q;
            while (temp % q == 0) temp /= q;
        }
    }
    if (temp > 1) {
        factors[fact_cnt++] = temp;                /* remaining prime factor */
    }
    factors[fact_cnt++] = p64;                    /* include p itself */

    /* reduce the order */
    for (size_t i = 0; i < fact_cnt; ++i) {
        uint64_t q = factors[i];
        while (order % q == 0) {
            uint64_t cand = order / q;
            if (pow_mod_uint64(10ULL, cand, mod64) == 1ULL) {
                order = cand;
            } else {
                break;
            }
        }
    }

    if (order % 2ULL != 0ULL) return 0;           /* order is odd – no -1 can appear */

    uint64_t exp = order / 2ULL;
    uint64_t val = pow_mod_uint64(10ULL, exp, mod64);
    if (val == (mod64 - 1ULL)) {
        return (seq_t)exp;                       /* exact when fits */
    } else {
        return 0;
    }
}
