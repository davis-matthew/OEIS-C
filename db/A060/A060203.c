/*
 * A060203  Least cube root of unity mod p, greater than 1, where p is the
 *          n-th prime congruent to 1 mod 3.
 * https://oeis.org/A060203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer >1 such that a(n)^3 ≡ 1 (mod p), where p is
 *            the n‑th prime ≡ 1 (mod 3)
 * Range:     u8 n <= 56, u16 n <= 6162, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*--- Modular exponentiation ------------------------------------------------*/
static inline uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

/*--- Tonelli–Shanks for odd primes ----------------------------------------*/
static uint64_t tonelli_shanks(uint64_t n, uint64_t p)
{
    if (n == 0) return 0;
    if (p == 2) return n;
    if (pow_mod_uint64(n, (p - 1) >> 1, p) != 1) return 0;   /* no square root */

    if (p % 4 == 3)
        return pow_mod_uint64(n, (p + 1) >> 2, p);

    /* Factor p-1 = Q * 2^S with Q odd */
    uint64_t Q = p - 1;
    uint64_t S = 0;
    while ((Q & 1ULL) == 0) {
        Q >>= 1;
        ++S;
    }

    /* Find a quadratic nonresidue z */
    uint64_t z = 2;
    while (pow_mod_uint64(z, (p - 1) >> 1, p) != p - 1)
        ++z;

    uint64_t c = pow_mod_uint64(z, Q, p);
    uint64_t R = pow_mod_uint64(n, (Q + 1) >> 1, p);
    uint64_t t = pow_mod_uint64(n, Q, p);
    uint64_t M = S;

    while (t != 1) {
        uint64_t i = 1;
        uint64_t t2 = t;
        while (i < M) {
            t2 = (unsigned __int128)t2 * t2 % p;
            if (t2 == 1) break;
            ++i;
        }
        uint64_t b = pow_mod_uint64(c, 1ULL << (M - i - 1), p);
        R = (unsigned __int128)R * b % p;
        t = (unsigned __int128)t * b % p;
        t = (unsigned __int128)t * b % p;
        c = (unsigned __int128)b * b % p;
        M = i;
    }
    return R;
}

/*--- Compute a(p) for a prime p ≡ 1 (mod 3) -------------------------------*/
static seq_t compute_a_for_prime(uint64_t p)
{
    /* Solve x^2 + x + 1 ≡ 0 (mod p)  →  x = (-1 ± sqrt(-3))/2 */
    uint64_t sqrt_neg3 = tonelli_shanks((p - 3) % p, p);   /* sqrt(-3) */
    uint64_t numerator = p - 1 + sqrt_neg3;                 /* (-1 + sqrt) mod p */
    if (numerator >= p) numerator -= p;
    uint64_t inv2 = (p + 1) / 2;                            /* inverse of 2 mod p */
    uint64_t r1 = (unsigned __int128)numerator * inv2 % p;  /* first root */
    uint64_t r2 = p - r1 - 1;                               /* second root */

    uint64_t ans = r1;
    if (ans < 2)
        ans = r2;
    else if (r2 < ans && r2 >= 2)
        ans = r2;
    return (seq_t)ans;
}

/*--- Prime handling -------------------------------------------------------*/
static uint64_t _current_candidate = 2;
static size_t   _primes_all_cap   = 0;
static size_t   _primes_all_cnt   = 0;
static uint64_t *_primes_all       = NULL;
static size_t   _cache_cap        = 0;
static size_t   _cache_cnt        = 0;
static seq_t   *_cache_vals       = NULL;

static void _grow_primes_all(size_t min_cap)
{
    if (_primes_all_cap >= min_cap) return;
    size_t new_cap = _primes_all_cap ? _primes_all_cap * 2 : 16;
    while (new_cap < min_cap) new_cap *= 2;
    _primes_all = (uint64_t *)realloc(_primes_all, new_cap * sizeof(uint64_t));
    _primes_all_cap = new_cap;
}

static void _grow_cache(size_t min_cap)
{
    if (_cache_cap >= min_cap) return;
    size_t new_cap = _cache_cap ? _cache_cap * 2 : 16;
    while (new_cap < min_cap) new_cap *= 2;
    _cache_vals = (seq_t *)realloc(_cache_vals, new_cap * sizeof(seq_t));
    _cache_cap = new_cap;
}

static int _is_prime(uint64_t n)
{
    if (n < 2) return 0;
    if (n == 2 || n == 3) return 1;
    if (n % 2 == 0) return 0;
    for (size_t i = 0; i < _primes_all_cnt; ++i) {
        uint64_t p = _primes_all[i];
        if (p * p > n) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

static void _add_prime(uint64_t p)
{
    _grow_primes_all(_primes_all_cnt + 1);
    _primes_all[_primes_all_cnt++] = p;
}

/*--- Main sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined for safety; offset starts at 1 */

    while (_cache_cnt < (size_t)n) {
        uint64_t cand = _current_candidate++;
        if (!_is_prime(cand)) continue;
        _add_prime(cand);
        if (cand % 3 == 1) {  /* needed prime */
            seq_t val = compute_a_for_prime(cand);
            _grow_cache(_cache_cnt + 1);
            _cache_vals[_cache_cnt++] = val;
        }
    }
    return _cache_vals[n - 1];
}
