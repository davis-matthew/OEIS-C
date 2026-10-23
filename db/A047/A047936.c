/*
 * A047936  Primes whose smallest positive primitive root (A001918) is not
 *          prime.
 * https://oeis.org/A047936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 630, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose smallest positive primitive root is not prime.
   The routine searches primes incrementally, caching results in a static
   array so that any call to a(n) returns the correct value. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- small primes sieve ---------- */
static uint32_t *small_primes = NULL;
static size_t small_primes_count = 0;

static void init_small_primes(void)
{
    if (small_primes != NULL) return;          /* already initialized */

    const size_t LIMIT = 1000000;
    uint8_t *is_comp = (uint8_t *)calloc(LIMIT + 1, 1);
    if (!is_comp) return;                      /* allocation failure */

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++small_primes_count;

    small_primes = (uint32_t *)malloc(small_primes_count * sizeof(uint32_t));
    if (!small_primes) {
        free(is_comp);
        small_primes_count = 0;
        return;
    }

    size_t k = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) small_primes[k++] = (uint32_t)i;

    free(is_comp);
}

/* ---------- modular arithmetic for 64‑bit integers ---------- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* ---------- primality test by trial division ---------- */
static bool is_prime_trial(uint64_t n)
{
    if (n < 2) return false;
    if (n < 4) return true;   /* 2, 3 */
    if ((n & 1) == 0) return false;

    init_small_primes();
    for (size_t i = 0; i < small_primes_count; ++i) {
        uint32_t p = small_primes[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ---------- factorisation into distinct prime factors ---------- */
static void factor_unique(uint64_t n, uint32_t *out_factors, size_t *out_count)
{
    *out_count = 0;
    init_small_primes();
    for (size_t i = 0; i < small_primes_count; ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) {
            out_factors[(*out_count)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) out_factors[(*out_count)++] = (uint32_t)n;
}

/* ---------- smallest primitive root of odd prime p (p>2) ---------- */
static uint64_t smallest_primitive_root(uint64_t p)
{
    uint32_t factors[64];
    size_t fcnt = 0;
    factor_unique(p - 1, factors, &fcnt);

    for (uint64_t g = 2; g < p; ++g) {
        bool ok = true;
        for (size_t i = 0; i < fcnt; ++i) {
            uint32_t q = factors[i];
            if (mod_pow(g, (p - 1) / q, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
    return 0;   /* should never happen for prime p > 2 */
}

/* ---------- sequence storage ---------- */
static seq_t *seq_primes = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t next_candidate = 3;   /* next integer to test */

/* append a new term */
static void seq_push(uint64_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(seq_primes, new_cap * sizeof(seq_t));
        if (!new_arr) return;   /* memory failure, ignore silently */
        seq_primes = new_arr;
        seq_cap = new_cap;
    }
    seq_primes[seq_len++] = (seq_t)val;
}

/* ---------- the required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    init_small_primes();

    /* initialise with the first term (prime 2) */
    if (seq_len == 0) {
        seq_push(2);
    }

    while (seq_len < (size_t)n) {
        uint64_t p = next_candidate;
        ++next_candidate;   /* prepare for next iteration */

        if (!is_prime_trial(p)) continue;

        /* p==2 is already accounted for */
        if (p == 2) continue;

        uint64_t g = smallest_primitive_root(p);
        if (g == 0) continue;   /* safety, should not happen */

        if (!is_prime_trial(g))
            seq_push(p);
    }

    return seq_primes[n - 1];
}
