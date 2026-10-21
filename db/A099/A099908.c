/*
 * A099908  a(n) = C(2n-1,n-1) mod n^4.
 * https://oeis.org/A099908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n-1, n-1) mod n^4 (offset 1)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 261, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   vp_fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/vp_fact.h"

/* --------------  helper: modular exponentiation (uint64_t) -------------- */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (uint64_t)((__uint128_t)result * base % mod);
        exp >>= 1ULL;
        if (exp)
            base = (uint64_t)((__uint128_t)base * base % mod);
    }
    return result;
}

/* --------------  primes up to 20000 ----------------------------------- */
#define PRIME_MAX 20000          /* 2*n-1 for n <= 10000  */
#define PRIME_CNT 2265           /* number of primes <=20000 */

static bool sieve_done = false;
static int primes[PRIME_CNT];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (sieve_done) return;

    static bool composite[PRIME_MAX + 1] = {false};
    prime_cnt = 0;

    for (int i = 2; i <= PRIME_MAX; ++i) {
        if (!composite[i]) {
            primes[prime_cnt++] = i;
            if ((int64_t)i * i <= PRIME_MAX) {
                for (int j = i * i; j <= PRIME_MAX; j += i)
                    composite[j] = true;
            }
        }
    }
    sieve_done = true;
}

/* --------------  main sequence function  ------------------------------ */
seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 */
    init_primes();

    /* modulus m = n^4, stored in 64‑bit (fits for n <= 10000) */
    uint64_t n64 = (uint64_t)n;
    uint64_t m64 = ((uint64_t)1 << 0);          /* dummy init */
    /* compute n^4 using 128‑bit to avoid overflow, then cast to u64 */
    __uint128_t tmp = (__uint128_t)n64 * n64 * n64 * n64;
    m64 = (uint64_t)tmp;                        /* fits in 64 bits for n<=10000 */

    /* special case: when m64 == 1 (i.e., n == 1) result is 0 */
    if (m64 == 1ULL) return (seq_t)0;

    uint64_t res = 1 % m64;

    uint64_t two_n_minus1 = 2 * n64 - 1;
    uint64_t n_minus1    = n64 - 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p > two_n_minus1) break;

        uint64_t exp = vp_fact(two_n_minus1, p)
                       - vp_fact(n_minus1, p)
                       - vp_fact(n64, p);

        if (exp == 0) continue;

        uint64_t pp = powmod64(p % m64, exp, m64);
        res = (uint64_t)((__uint128_t)res * pp % m64);
    }

    return (seq_t)res;
}
