/*
 * A132632  Minimal m > 0 such that Fibonacci(m) == 0 (mod n^2).
 * https://oeis.org/A132632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   compute_rank_prime, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = minimal m > 0 such that Fib(m) ≡ 0 (mod n²)
 * (OEIS A132632).  The implementation factors n, computes the
 * rank of each prime power dividing n² using known formulas,
 * and returns the least common multiple of these ranks.  All
 * computation uses only types allowed by the build (seq_t for
 * sequence values, idx_t for indices), and no external libraries
 * beyond the provided helpers.  */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/compute_rank_prime.h"

static bool primes_inited = false;
static uint32_t primes[1229];
static size_t num_primes;

/* Cache for rank(p) of a prime  p ≤ 10000 (p ≠ 2,5) */
static seq_t rank_prime_cache[10001];
static bool rank_prime_done[10001] = { false };

/* --- Helper: Sieve primes up to 10000 ------------------------------------ */
static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;
    bool comp[10001] = { false };
    num_primes = 0;
    for (uint32_t i = 2; i <= 10000; ++i) {
        if (!comp[i]) {
            primes[num_primes++] = i;
            if ((uint64_t)i * i <= 10000) {
                for (uint32_t j = i * i; j <= 10000; j += i)
                    comp[j] = true;
            }
        }
    }
}

/* --- Helper: Integer power (idx_t base, uint32_t exp) --------------------- */
static seq_t pow_uint_seq(idx_t base, uint32_t exp)
{
    idx_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return (seq_t)res;
}

/* --- Rank of p^k (k ≥ 1) ------------------------------------------------- */
static seq_t rank_prime_power(uint32_t p, uint32_t k)
{
    if (p == 2) {                   /* 2^k */
        if (k == 1) return 3;       /* 2^1  → 3 */
        if (k == 2) return 6;       /* 2^2  → 6 */
        return 3 * pow_uint_seq(2, k - 2);   /* k ≥ 3 : 3·2^(k‑2) */
    }
    if (p == 5)                      /* 5^k */
        return pow_uint_seq(5, k);   /* rank = 5^k */

    /* odd prime p ≠ 5 */
    if (!rank_prime_done[p]) {
        rank_prime_cache[p] = compute_rank_prime(p);
        rank_prime_done[p] = true;
    }
    seq_t rank_p = rank_prime_cache[p];
    return rank_p * pow_uint_seq(p, k - 1);
}

/* --- Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1) return 1;           /* a(1) = 1 (and works for n=0 as unused) */

    init_primes();

    seq_t lcm_result = 1;
    uint32_t tmp = (uint32_t)n;

    /* Factor n, build ranks for each prime power in n². */
    for (size_t i = 0; i < num_primes && primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            uint32_t k = 2 * e;                /* exponent in n² */
            seq_t rk = rank_prime_power(p, k);
            seq_t g  = gcd(lcm_result, rk);
            lcm_result = (lcm_result / g) * rk;
        }
    }

    /* Remaining prime factor > sqrt(tmp) */
    if (tmp > 1) {
        uint32_t p = tmp;
        uint32_t e = 1;                 /* exponent in n */
        uint32_t k = 2 * e;
        seq_t rk = rank_prime_power(p, k);
        seq_t g  = gcd(lcm_result, rk);
        lcm_result = (lcm_result / g) * rk;
    }

    return lcm_result;
}
