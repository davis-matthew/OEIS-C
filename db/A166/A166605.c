/*
 * A166605  Numbers k such that Sum_{i=1..k} i^5 divides Product_{i=1..k} i^5.
 * https://oeis.org/A166605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th member of OEIS sequence A166605:
   the numbers k such that Σ_{i=1..k} i^5 divides Π_{i=1..k} i^5. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/* Small primes for trial division (up to 1000). */
static const uint16_t small_primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,
    157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,
    239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,
    331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,
    421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,
    509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,
    613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,
    709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,
    821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,
    919,929,937,941,947,953,967,971,977,983,991,997
};
static const size_t small_primes_cnt = sizeof(small_primes) / sizeof(small_primes[0]);

/* 64‑bit GCD -------------------------------------------------------- */

/* Modular multiplication using 128‑bit intermediate. */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

/* Modular exponentiation. */
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    unsigned __int128 result = 1;
    unsigned __int128 b = base % mod;
    while (exp) {
        if (exp & 1) result = result * b % mod;
        b = b * b % mod;
        exp >>= 1;
    }
    return (uint64_t)result;
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_64(uint64_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < small_primes_cnt && (uint64_t)small_primes[i] * small_primes[i] <= n; ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    const uint64_t bases[] = {
        2, 325, 9375, 28178, 450775, 9780504, 1795265022
    };
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Pollard–Rho factorization (Brent's variant). */
static uint64_t pollard_rho(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mul_mod(x, x, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_uint64(diff, n);
        }
        if (d != n) return d;
        ++c;
    }
}

/* Prime factor + exponent structure. */
typedef struct { uint64_t prime; uint32_t exp; } prime_exp_t;

/* Append or increment an exponent for a prime. */
static void add_exp(prime_exp_t **arr, size_t *len, size_t *cap,
                    uint64_t p, uint32_t inc) {
    for (size_t i = 0; i < *len; ++i) {
        if ((*arr)[i].prime == p) {
            (*arr)[i].exp += inc;
            return;
        }
    }
    if (*len == *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *arr = (prime_exp_t *)realloc(*arr, *cap * sizeof(prime_exp_t));
    }
    (*arr)[*len].prime = p;
    (*arr)[*len].exp = inc;
    ++(*len);
}

/* Recursive factorisation. */
static void factor_rec(uint64_t n, prime_exp_t **arr, size_t *len, size_t *cap,
                       uint32_t mul) {
    if (n < 2) return;
    if (is_prime_64(n)) {
        add_exp(arr, len, cap, n, mul);
        return;
    }
    uint64_t d = pollard_rho(n);
    factor_rec(d, arr, len, cap, mul);
    factor_rec(n / d, arr, len, cap, mul);
}

/* Factorisation with multiplicity 'mul'. */
static void factor_with_mul(uint64_t n, prime_exp_t **arr, size_t *len,
                            size_t *cap, uint32_t mul) {
    if (n < 2) return;
    for (size_t i = 0; i < small_primes_cnt; ++i) {
        uint64_t p = small_primes[i];
        if (p * p > n) break;
        while (n % p == 0) {
            add_exp(arr, len, cap, p, mul);
            n /= p;
        }
    }
    if (n == 1) return;
    if (is_prime_64(n)) {
        add_exp(arr, len, cap, n, mul);
        return;
    }
    factor_rec(n, arr, len, cap, mul);
}

/* Legendre exponent of prime p in k!. */
static uint32_t legendre_exp(idx_t k, uint64_t p) {
    uint32_t e = 0;
    idx_t t = k;
    while (t) { t /= p; e += (uint32_t)t; }
    return e;
}

/* Test whether a given k satisfies the divisibility condition. */
static bool valid_k(idx_t k) {
    prime_exp_t *exp = NULL;
    size_t exp_len = 0, exp_cap = 0;

    /* k^5 contributes (k^2)*(k^3). The exponent counting is simpler:
       factor k twice (for k^2), (k+1) twice, and Q = 2k^2+2k-1 once.
       Then subtract the factor 12 = 2^2*3. */
    factor_with_mul((uint64_t)k, &exp, &exp_len, &exp_cap, 2);
    factor_with_mul((uint64_t)(k + 1), &exp, &exp_len, &exp_cap, 2);
    uint64_t Q = 2ULL * k * k + 2ULL * k - 1ULL;
    factor_with_mul(Q, &exp, &exp_len, &exp_cap, 1);

    /* cancel 12 */
    for (size_t i = 0; i < exp_len; ++i) {
        if (exp[i].prime == 2) {
            exp[i].exp = exp[i].exp >= 2 ? exp[i].exp - 2 : 0;
        } else if (exp[i].prime == 3) {
            exp[i].exp = exp[i].exp >= 1 ? exp[i].exp - 1 : 0;
        }
    }

    /* now all remaining exponents must satisfy eS <= 5*eFact(k,p) */
    bool ok = true;
    for (size_t i = 0; i < exp_len && ok; ++i) {
        uint64_t p = exp[i].prime;
        uint32_t eS = exp[i].exp;
        if (eS == 0) continue;
        uint32_t eFact = legendre_exp(k, p);
        if (eS > 5u * eFact) ok = false;
    }
    free(exp);
    return ok;
}

/* ------------------------------------------------------------------ */
/*   Main sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0, len = 0;
    static idx_t next_k = 1;

    while (len < n) {
        if (valid_k(next_k)) {
            if (len == cap) {
                cap = (cap == 0) ? 16 : cap << 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
