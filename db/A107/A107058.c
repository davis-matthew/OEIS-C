/*
 * A107058  a(n) = smallest number m>0 such that prime(n)*prime(n+1)-m is a
 *          prime.
 * https://oeis.org/A107058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107058: a(n) = smallest m>0 such that prime(n)*prime(n+1)-m is prime.
 *  Computation method: find the largest prime less than p_n * p_{n+1}
 *  where p_k is the k‑th prime; then a(n) = p_n * p_{n+1} - that prime.   */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- prime utility (deterministic Miller–Rabin for 64‑bit) ---------------- */
static inline uint64_t modmul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static inline uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = modmul_uint64(result, base, mod);
        base = modmul_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    /* trial division by first few primes */
    const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,
                                   17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* Miller–Rabin with deterministic bases for 64‑bit */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,
                              450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = modmul_uint64(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---- dynamic list of primes --------------------------------------------- */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static void ensure_primes(size_t need)
{
    if (primes_cnt >= need) return;
    /* grow capacity */
    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }
    while (primes_cap < need) {
        primes_cap *= 2;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }
    /* find new primes */
    uint64_t candidate = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
    if (candidate > 2 && (candidate & 1) == 0) ++candidate; /* make odd */
    while (primes_cnt < need) {
        if (isPrime64(candidate)) {
            primes[primes_cnt++] = candidate;
        }
        candidate += (candidate == 2) ? 1 : 2; /* next odd */
    }
}

/* ---- sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n<1, but guard */
    ensure_primes((size_t)(n + 1));     /* need prime(n) and prime(n+1) */
    const uint64_t p_n     = primes[(size_t)n  - 1];
    const uint64_t p_nplus = primes[(size_t)n];
    const uint64_t product = p_n * p_nplus;   /* fits in 64‑bit for n≤10000 */

    uint64_t m = 0;
    /* look downward for the largest prime below product */
    for (uint64_t cand = product - 1;; --cand) {
        if (isPrime64(cand)) {
            m = product - cand;
            break;
        }
        /* the loop is guaranteed to terminate because 2 is prime */
    }
    return (seq_t)m;
}
