/*
 * A067843  Least solution k>n of phi(k-n)+phi(k+n)=phi(2k).
 * https://oeis.org/A067843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least k > n such that φ(k−n) + φ(k+n) = φ(2k)
 * (computed by brute‑force search with a growing φ table)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- static tables for φ and a linear sieve ----- */
static size_t phi_limit = 0;          /* max n for which φ is known          */
static uint64_t *phi_values = NULL;   /* φ[0..phi_limit]                     */
static uint32_t *spf = NULL;          /* smallest prime factor for each n    */
static uint32_t *primes = NULL;       /* list of primes found                */
static size_t primes_cnt = 0;         /* number of primes stored             */
static size_t primes_cap = 0;         /* capacity of the primes array        */

/* ---------- helpers ---------- */

/* enlarge the φ table to cover all numbers <= new_limit          */
static void extend_phi(size_t new_limit)
{
    if (new_limit <= phi_limit) return;

    size_t old_limit = phi_limit;
    phi_limit = new_limit;

    /* reallocate φ array (C++ requires a cast) */
    uint64_t *new_phi = (uint64_t *)realloc(phi_values,
                                            (phi_limit + 1) * sizeof(uint64_t));
    if (!new_phi) exit(EXIT_FAILURE);
    phi_values = new_phi;

    /* reallocate spf array */
    uint32_t *new_spf = (uint32_t *)realloc(spf,
                                            (phi_limit + 1) * sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);
    spf = new_spf;

    /* initialise if first time */
    if (old_limit == 0) {
        spf[0] = 0;
        spf[1] = 1;
        phi_values[0] = 0;
        phi_values[1] = 1;
        primes_cap = 64;
        primes = (uint32_t *)calloc(primes_cap, sizeof(uint32_t));
        if (!primes) exit(EXIT_FAILURE);
        primes_cnt = 0;
    }

    /* set spf to 0 for new entries */
    for (size_t i = old_limit + 1; i <= phi_limit; ++i) spf[i] = 0;

    /* linear sieve up to phi_limit */
    for (size_t i = 2; i <= phi_limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap * 2;
                uint32_t *tmp = (uint32_t *)realloc(primes,
                                                     primes_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
            phi_values[i] = i - 1;
        }

        for (size_t j = 0; j < primes_cnt &&
                         primes[j] <= spf[i] &&
                         i * (size_t)primes[j] <= phi_limit; ++j) {
            size_t p  = primes[j];
            size_t ip = i * p;
            spf[ip] = (uint32_t)p;
            if (i % p == 0)
                phi_values[ip] = phi_values[i] * p;
            else
                phi_values[ip] = phi_values[i] * (p - 1);
        }
    }
}

/* return φ(n); n is assumed to be <= phi_limit (otherwise it will be
   computed by factorisation using the current prime table).          */
static uint64_t phi_uint64(uint64_t n)
{
    if (n <= phi_limit) return phi_values[(size_t)n];

    uint64_t result = n;
    uint64_t tmp = n;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] *
         (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1) result -= result / tmp;
    return result;
}

/* ---------- public function required by the specification ---------- */

/*
 * a(idx_t n) returns the least k > n with φ(k−n)+φ(k+n)=φ(2k).
 * The function works for any n ≥ 1 and can be called in any order.
 */
extern seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    /* start searching from n+1 */
    size_t k = (size_t)n + 1;

    while (true) {
        /* ensure the φ table covers all numbers needed for this k */
        extend_phi(2 * k);

        uint64_t lhs = phi_uint64((uint64_t)k - (uint64_t)n) +
                       phi_uint64((uint64_t)k + (uint64_t)n);
        uint64_t rhs = phi_uint64(2 * (uint64_t)k);

        if (lhs == rhs) return (seq_t)k;
        ++k;
    }
}
