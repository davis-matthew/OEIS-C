/*
 * A080715  Numbers k such that for any positive integers (a, b), if a * b = k
 *          then a + b is prime.
 * https://oeis.org/A080715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 228, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A080715 – numbers k such that for every factorisation a*b = k, a+b is prime.
   The function finds each term by a simple brute‑force search: the
   next integer is tested for (i) evenness, (ii) square‑freeness, and
   (iii) primality of all sums a+b of its divisor pairs.  The results
   are stored in a static array so that every n ≥ 1 can be answered
   in O(1) after the initial search.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- primality test for 64‑bit integers (deterministic) ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint32_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n-1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }
    static const uint32_t bases[] = {
        2, 325, 9375, 28178, 450775, 9780504, 1795265022
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- dynamic prime sieve for factorisation ---------- */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;
static uint32_t sieve_limit = 1;   /* primes generated up to this number */

static void extend_prime_sieve(uint32_t limit)
{
    if (limit <= sieve_limit) return;
    if (prime_capacity < limit+16) {
        prime_capacity = limit+16;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_capacity * sizeof(uint32_t));
        if (!tmp) exit(1);
        prime_list = tmp;
    }
    for (uint32_t n = sieve_limit+1; n <= limit; ++n) {
        bool prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > n) break;
            if (n % p == 0) { prime = false; break; }
        }
        if (prime) {
            prime_list[prime_count++] = n;
            if (prime_count == prime_capacity) {
                prime_capacity <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_capacity * sizeof(uint32_t));
                if (!tmp) exit(1);
                prime_list = tmp;
            }
        }
    }
    sieve_limit = limit;
}

/* ---------- factorisation: distinct primes, check for square‑free ---------- */
static bool factor_squarefree(uint64_t k, uint32_t *factors, size_t *num_factors)
{
    *num_factors = 0;
    if (k == 1) return true;
    uint64_t tmp = k;
    uint32_t lim = (uint32_t)(sqrt((double)tmp) + 1.0);
    extend_prime_sieve(lim);
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i]*prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            size_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
                if (cnt > 1) return false;   /* square factor detected */
            }
            factors[(*num_factors)++] = p;
        }
    }
    if (tmp > 1) {
        factors[(*num_factors)++] = (uint32_t)tmp;
    }
    return true;
}

/* ---------- test all divisor pairs recursively ---------- */
static bool divisors_ok(const uint32_t *factors, size_t nf, size_t idx,
                        uint64_t cur, uint64_t k)
{
    if (idx == nf) {
        uint64_t d = cur;
        uint64_t sum = d + k / d;
        return is_prime_uint64(sum);
    }
    /* include this prime */
    if (!divisors_ok(factors, nf, idx+1, cur * factors[idx], k))
        return false;
    /* exclude this prime */
    if (!divisors_ok(factors, nf, idx+1, cur, k))
        return false;
    return true;
}

/* ---------- test whether k belongs to the sequence ---------- */
static bool good_candidate(uint64_t k)
{
    if (k == 1) return true;
    if ((k & 1ULL) != 0) return false;  /* must be even */
    uint32_t facs[32];
    size_t nf;
    if (!factor_squarefree(k, facs, &nf)) return false;
    return divisors_ok(facs, nf, 0, 1ULL, k);
}

/* ---------- memoised table of sequence values ---------- */
#define TERMS_CAP 20000   /* enough for n up to ~10 000 */
static seq_t  table[TERMS_CAP];
static size_t table_size = 0;
static uint64_t next_k = 1;   /* smallest integer to test next */

/* ---------- public function required by the specification ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined but harmless */

    while (table_size < (size_t)n) {
        if (good_candidate(next_k)) {
            if (table_size >= TERMS_CAP) exit(1);   /* safety guard */
            table[table_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return table[n-1];
}
