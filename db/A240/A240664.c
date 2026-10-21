/*
 * A240664  Least k such that 9^k == -1 (mod prime(n)), or 0 if no such k
 *          exists.
 * https://oeis.org/A240664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that 9^k ≡ -1 (mod prime(n)). Computation:
 *            Find the multiplicative order ord of 9 modulo prime(n). If ord
 *            is even and 9^(ord/2) ≡ -1, return ord/2; otherwise return 0
 * Range:     u8 n <= 175, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* Small primes up to 400, enough to trial‑factor p-1 for p ≤ ~100000. */
static const uint64_t SMALL_PRIMES[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL,
    29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL,
    61ULL, 67ULL, 71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL,
   101ULL, 103ULL, 107ULL, 109ULL, 113ULL, 127ULL, 131ULL,
   137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL, 167ULL,
   173ULL, 179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL,
   211ULL, 223ULL, 227ULL, 229ULL, 233ULL, 239ULL, 241ULL,
   251ULL, 257ULL, 263ULL, 269ULL, 271ULL, 277ULL, 281ULL,
   283ULL, 293ULL, 307ULL, 311ULL, 313ULL, 317ULL, 331ULL,
   337ULL, 347ULL, 349ULL, 353ULL, 359ULL, 367ULL, 373ULL,
   379ULL, 383ULL, 389ULL, 397ULL
};
static const size_t SMALL_PRIMES_CNT = sizeof(SMALL_PRIMES) / sizeof(SMALL_PRIMES[0]);

/* Custom modular exponentiation using 128‑bit to avoid overflow. */
static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
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

/* Compute the multiplicative order of 9 modulo prime p (p prime, p > 3). */
static uint64_t order_of_9(uint64_t p)
{
    uint64_t ord = p - 1;
    uint64_t m   = ord;

    for (size_t i = 0; i < SMALL_PRIMES_CNT && SMALL_PRIMES[i] * SMALL_PRIMES[i] <= m; ++i) {
        uint64_t q = SMALL_PRIMES[i];
        if (m % q != 0) continue;

        /* Remove q completely from factorisation of (p-1). */
        while (m % q == 0) m /= q;

        /* Eliminate q from ord as long as it remains a factor and 9^(ord/q) ≡ 1. */
        while (ord % q == 0 &&
               modpow64(9, ord / q, p) == 1)
            ord /= q;
    }

    /* If a prime factor remains (m > 1). */
    if (m > 1) {
        uint64_t q = m;
        while (ord % q == 0 &&
               modpow64(9, ord / q, p) == 1)
            ord /= q;
    }

    return ord;
}

seq_t a(idx_t n)
{
    /* OEIS offset 1: a(1) corresponds to prime(1) = 2. */
    if (n == 0) return (seq_t)0; /* undefined for n < 1 */

    uint64_t p = get_prime(n);

    /* Special handling for small primes */
    if (p == 2ULL) return (seq_t)1;      /* 9^1 ≡ 1 ≡ -1 (mod 2) */
    if (p == 3ULL) return (seq_t)0;      /* 9 ≡ 0 (mod 3) never equals -1 */

    uint64_t ord = order_of_9(p);
    if (ord & 1ULL) return (seq_t)0;     /* ord odd → -1 not in subgroup */

    uint64_t half = ord >> 1;
    if (modpow64(9, half, p) == p - 1ULL)
        return (seq_t)half;              /* k = ord/2 */

    return (seq_t)0;
}
