/*
 * A069082  Numbers n such that sigma(n)/tau(n) has denominator 3.
 * https://oeis.org/A069082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth integer m ≥ 1 for which the reduced fraction
 *            σ(m)/τ(m) has denominator exactly 3. The algorithm tests
 *            successive integers, factors them using a simple incremental
 *            prime sieve, computes σ and τ, and checks the gcd‑based
 *            condition described in the comments
 * Range:     u8 n <= 5, u16 n <= 555, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- Prime sieve helpers ---------- */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;
static uint32_t prime_last_checked = 1;

/* initialise the prime list with the first prime 2 */
static void init_primes(void)
{
    if (prime_count == 0) {
        prime_cap = 8;
        prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        prime_list[0] = 2;
        prime_count = 1;
        prime_cap = 8;
        prime_last_checked = 2;
    }
}

/* ensure all primes ≤ limit are in prime_list */
static void ensure_primes_upto(uint32_t limit)
{
    if (limit <= prime_last_checked)
        return;

    for (uint32_t cand = prime_last_checked + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap * 2 + 1;
                prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_count++] = cand;
        }
    }
    prime_last_checked = limit;
}

/* integer square root, floor value */
static uint32_t isqrt_uint64(uint64_t n)
{
    uint64_t x = (uint64_t) sqrt((long double) n);
    while ((x + 1) * (x + 1) <= n) ++x;
    while (x * x > n) --x;
    return (uint32_t)x;
}

/* ---------- Good‑n test ---------- */
static bool good_n(idx_t n)
{
    /* compute σ(n) and τ(n) */
    seq_t sigma = 1;
    uint64_t tau  = 1;
    uint64_t m = (uint64_t)n;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > m)
            break;
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* σ factor = (p^(exp+1)-1)/(p-1) */
            seq_t pow = 1;
            for (int j = 0; j <= exp; ++j)
                pow *= (seq_t)p;
            seq_t sigma_factor = (pow - 1) / ((seq_t)p - 1);
            sigma *= sigma_factor;
            tau   *= (uint64_t)(exp + 1);
        }
    }
    if (m > 1) {            /* remaining prime factor, exponent 1 */
        uint32_t p = (uint32_t)m;
        seq_t sigma_factor = (seq_t)p + 1;   /* (p^2-1)/(p-1) = p+1 */
        sigma *= sigma_factor;
        tau   *= 2;
    }

    if (tau % 3 != 0)
        return false;

    /* k = τ/3, extract 3‑power */
    uint64_t k = tau / 3;
    unsigned f = 0;
    uint64_t kt = k;
    while (kt % 3 == 0) {
        kt /= 3;
        ++f;
    }
    uint64_t kprime = kt;            /* h in the earlier explanation */

    /* exponent of 3 in σ(n) */
    seq_t temp_sigma = sigma;
    unsigned sv3 = 0;
    while (temp_sigma % 3 == 0) {
        temp_sigma /= 3;
        ++sv3;
    }
    if (sv3 != f)
        return false;

    /* σ must be divisible by kprime */
    if (kprime != 0 && (sigma % (seq_t)kprime) != 0)
        return false;

    return true;
}

/* ---------- Sequence table ---------- */
static seq_t *terms = NULL;
static size_t term_count = 0;
static size_t term_cap   = 0;
static idx_t next_candidate = 1;

/* grow the terms array if needed */
static void ensure_term_cap(size_t need)
{
    if (term_cap >= need)
        return;
    while (term_cap < need) {
        term_cap = term_cap * 2 + 1;
    }
    terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    init_primes();

    /* generate terms until we have the nth one */
    while (term_count < (size_t)n) {
        uint32_t limit = isqrt_uint64((uint64_t)next_candidate) + 1;
        if (prime_list[prime_count - 1] < limit)
            ensure_primes_upto(limit);

        if (good_n(next_candidate)) {
            ensure_term_cap(term_count + 1);
            terms[term_count++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return terms[(size_t)n - 1];
}
