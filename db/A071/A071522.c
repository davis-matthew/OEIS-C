/*
 * A071522  Numbers n such that x^n + x^(n-1) + x^(n-2) + ... + x + 1 is
 *          irreducible over GF(5).
 * https://oeis.org/A071522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p - 1, where p = nth prime such that 5 is a primitive
 *            root modulo p (p = 2 counts as good, p = 5 does not)
 * Range:     u8 n <= 24, u16 n <= 2589, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*---------- helper: modular exponentiation for arbitrary seq_t ----------*/
static seq_t mod_pow_seq(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (seq_t)((unsigned __int128)result * base % mod);
        base = (seq_t)((unsigned __int128)base * base % mod);
        exp >>= 1ULL;
    }
    return result;
}

/*---------- dynamic array of uint64_t primes ----------*/
static uint64_t *prime_list   = NULL;
static size_t    prime_cnt    = 0;
static size_t    prime_cap    = 0;

static void prime_reserve(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

static void prime_push(uint64_t p)
{
    prime_reserve(prime_cnt + 1);
    prime_list[prime_cnt++] = p;
}

/*---------- dynamic array of seq_t values that satisfy the condition ----------*/
static seq_t *good_vals   = NULL;
static size_t  good_cnt    = 0;
static size_t  good_cap    = 0;
static size_t  prime_idx   = 0; /* next prime index to process */

static void good_reserve(size_t need)
{
    if (need <= good_cap) return;
    size_t new_cap = good_cap ? good_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(good_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    good_vals = tmp;
    good_cap  = new_cap;
}

static void good_push(seq_t v)
{
    good_reserve(good_cnt + 1);
    good_vals[good_cnt++] = v;
}

/*---------- primality test using existing prime list (trial division) ----------*/
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if ((unsigned __int128)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/*---------- test whether 5 is a primitive root modulo p ----------*/
static bool is_primitive_root5(uint64_t p)
{
    if (p == 2)             return true;  /* 5 ≡ 1 (mod 2) → order 1 = p‑1 */
    if (p == 5)             return false; /* 5 ≡ 0 (mod 5) → never primitive */

    uint64_t m = p - 1;
    uint64_t t = m;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_list[i];
        if ((unsigned __int128)q * q > t) break;
        if (t % q == 0) {
            if (mod_pow_seq((seq_t)5, (p - 1) / q, (seq_t)p) == 1) return false;
            while (t % q == 0) t /= q;
        }
    }
    if (t > 1) /* remaining prime factor */
        if (mod_pow_seq((seq_t)5, (p - 1) / t, (seq_t)p) == 1) return false;
    return true;
}

/*---------- sieve to generate primes up to a fixed limit ----------*/
static bool sieve_done   = false;
static const size_t SIEVE_LIMIT = 2000000; /* sufficient for n≈10000 */

static void build_sieve(void)
{
    if (sieve_done) return;
    bool *is_comp = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_comp) abort();
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = true;
        }
    }
    prime_reserve(200000); /* rough reserve */
    prime_push(2);
    for (size_t i = 3; i <= SIEVE_LIMIT; i += 2)
        if (!is_comp[i]) prime_push((uint64_t)i);
    free(is_comp);
    sieve_done = true;
}

/*---------- generate good values up to a given n ----------*/
static void generate_needed(idx_t n)
{
    build_sieve();
    while (good_cnt < (size_t)n) {
        if (prime_idx >= prime_cnt) abort(); /* should not happen with large limit */
        uint64_t p = prime_list[prime_idx++];
        if (is_primitive_root5(p))
            good_push((seq_t)(p - 1));
    }
}

/*---------- required function -----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined; return 0 per spec */
    generate_needed(n);
    return good_vals[n - 1];
}
