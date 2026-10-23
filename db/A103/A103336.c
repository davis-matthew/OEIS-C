/*
 * A103336  Numbers whose largest primitive root (A046146) is not prime.
 * https://oeis.org/A103336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 9022, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose largest primitive root (A046146) is not prime.
   Computes the n‑th term by scanning integers, testing whether the largest
   primitive root of each is composite (or 1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- Prime sieve and helpers ---------- */
#define MAX_PRIME 2000000          /* enough for n up to many thousands */
#define PRIME_CAP 100000

static uint64_t primes[PRIME_CAP];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    static bool built = false;
    if (built) return;
    bool *is_composite = (bool *)calloc(MAX_PRIME + 1, sizeof(bool));
    size_t idx = 0;
    for (uint64_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_composite[(size_t)i]) {
            if (idx < PRIME_CAP) primes[idx++] = i;
            if (i * i <= MAX_PRIME) {
                for (uint64_t j = i * i; j <= MAX_PRIME; j += i)
                    is_composite[(size_t)j] = true;
            }
        }
    }
    prime_cnt = idx;
    free(is_composite);
    built = true;
}

/* 64‑bit GCD */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Modular exponentiation (base^exp mod mod), safe for 64‑bit values */
static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1) {
            __uint128_t mul = (__uint128_t)result * base;
            result = (uint64_t)(mul % mod);
        }
        __uint128_t mul = (__uint128_t)base * base;
        base = (uint64_t)(mul % mod);
        exp >>= 1;
    }
    return result;
}

/* Simple primality test using the prime list */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return n == p;
    }
    return true;
}

/* Factorise n into distinct primes and exponents.
   Returns number of distinct primes. */
static int factor_uint64(uint64_t n, uint64_t *pf, int *exp)
{
    int cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) {
            pf[cnt] = p;
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (n > 1) {
        pf[cnt] = n;
        exp[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* Test whether a group modulo n is cyclic. */
static bool has_primitive_root(uint64_t n)
{
    if (n == 1 || n == 2 || n == 4) return true;
    uint64_t m = n;
    int twos = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        ++twos;
    }
    if (twos == 0 || twos == 1) {
        /* n = p^k or 2*p^k where p is odd prime */
        uint64_t pf[20];
        int exp[20];
        int cnt = factor_uint64(m, pf, exp);
        return cnt == 1;
    }
    return false;
}

/* Compute the largest primitive root modulo n (assuming n has one). */
static uint64_t largest_primitive_root(uint64_t n)
{
    if (n <= 2) return 1;          /* only n=1,2 have trivial g=1 */

    /* Compute φ(n) */
    uint64_t phi = n;
    uint64_t pf_n[20];
    int exp_n[20];
    int cnt_n = factor_uint64(n, pf_n, exp_n);
    for (int i = 0; i < cnt_n; ++i)
        phi -= phi / pf_n[i];

    /* Factor φ(n) */
    uint64_t pf_phi[20];
    int exp_phi[20];
    int cnt_phi = factor_uint64(phi, pf_phi, exp_phi);

    /* Search candidate g from n-1 downwards */
    for (uint64_t g = n - 1; g > 1; --g) {
        if (gcd64(g, n) != 1) continue;
        bool ok = true;
        for (int i = 0; i < cnt_phi; ++i) {
            uint64_t e = phi / pf_phi[i];
            if (modpow_uint64(g, e, n) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
    return 1; /* should never reach */
}

/* Does n qualify? (largest primitive root not prime) */
static bool qualifies(uint64_t n)
{
    if (n == 1 || n == 2) return true;   /* by convention */
    if (!has_primitive_root(n)) return false;
    uint64_t g = largest_primitive_root(n);
    return !is_prime_uint64(g);
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    static seq_t *term_list = NULL;
    static size_t list_cap = 0, list_len = 0;
    static uint64_t cur = 1;

    init_primes();

    while (list_len < (size_t)n) {
        if (qualifies(cur)) {
            if (list_len == list_cap) {
                size_t new_cap = list_cap ? list_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(term_list, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                term_list = tmp;
                list_cap = new_cap;
            }
            term_list[list_len++] = (seq_t)cur;
        }
        ++cur;
    }
    return term_list[n - 1];
}
