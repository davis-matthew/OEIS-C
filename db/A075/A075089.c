/*
 * A075089  Smallest prime == 1 mod n-th composite number.
 * https://oeis.org/A075089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A075089:  
   a(n) = the smallest prime p such that p ≡ 1 (mod c_n),
   where c_n is the n‑th composite number (c_1 = 4).  The algorithm
   does not hard‑code terms: it generates composites on demand
   and for each new composite finds the first prime congruent to 1
   modulo that composite, using a deterministic Miller–Rabin test
   for 64‑bit integers.  All intermediate
   arithmetic stays within machine integer ranges, since the
   composites and primes needed for n ≤ 10 000 are far below 2³².  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Miller–Rabin primality test (deterministic for 64‑bit) ---------- */

/* modular multiplication */
static bool mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod, uint64_t *res)
{
    *res = (uint64_t)((__uint128_t)a * b % mod);
    return true;
}

/* modular exponentiation */
static bool pow_mod_u64(uint64_t a, uint64_t d, uint64_t mod, uint64_t *res)
{
    __uint128_t r = 1;
    __uint128_t base = a % mod;
    while (d) {
        if (d & 1) r = r * base % mod;
        base = base * base % mod;
        d >>= 1;
    }
    *res = (uint64_t)r;
    return true;
}

/* Miller–Rabin test for 64‑bit integers */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    static const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x;
        pow_mod_u64(a, d, n, &x);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            mul_mod_u64(x, x, n, &x);
            if (x == n - 1) { cont = true; break; }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* ---------- Cached data structures ---------- */
static seq_t *composites = NULL;       /* array of composites (c_n)      */
static seq_t *results   = NULL;       /* array of a(n) values           */
static size_t comp_len = 0;           /* number of composites stored    */
static size_t comp_cap = 0;           /* capacity of the arrays         */
static uint64_t next_candidate = 4;   /* next integer to test for compositeness */

/* Ensure the arrays can hold at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (need <= comp_cap) return;
    size_t new_cap = comp_cap ? comp_cap * 2 : 128;
    while (new_cap < need) new_cap *= 2;
    composites = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
    results   = (seq_t *)realloc(results,   new_cap * sizeof(seq_t));
    comp_cap = new_cap;
}

/* Generate composites up to index `n` (1‑based). For each new composite,
   compute the corresponding a(n). */
static void generate_up_to(idx_t n)
{
    size_t target = (size_t)n;
    ensure_capacity(target);
    while (comp_len < target) {
        /* find the next composite number */
        while (isPrime64(next_candidate)) ++next_candidate;
        composites[comp_len] = (seq_t)next_candidate;   /* store composite */
        /* find the smallest prime ≡ 1 (mod composite) */
        uint64_t m = (uint64_t)next_candidate;
        uint64_t candidate;
        for (uint64_t k = 1;; ++k) {
            candidate = m * k + 1U;
            if (isPrime64(candidate)) {
                results[comp_len] = (seq_t)candidate;
                break;
            }
        }
        ++comp_len;
        ++next_candidate;    /* prepare for next composite */
    }
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of bounds according to the offset */
    generate_up_to(n);
    return results[(size_t)n - 1];
}
