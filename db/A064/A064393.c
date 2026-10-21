/*
 * A064393  Numbers k such that the exponent of highest power of 2 dividing k!
 *          equals the largest prime <= k.
 * https://oeis.org/A064393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) is the n-th integer k such that * v2(k!) = Σ_{i≥1} ⌊k/2^i⌋
 *            equals the greatest prime ≤ k
 * Range:     u8 n <= 31, u16 n <= 3247, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- primality utilities (deterministic for 64‑bit) ---------- */

/* modular multiplication (a*b mod m), safe for 64‑bit */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((__uint128_t)a * b % m);
}

/* modular exponentiation (base^exp mod mod) */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Miller–Rabin primality test, deterministic for n < 2^64
   bases: 2, 3, 5, 7, 11, 13, 17  */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* largest prime ≤ n (for n≥2 → 0 if no such prime). */
static uint64_t prevprime_uint64(uint64_t n) {
    if (n <= 2) return 0;           /* no prime ≤ 1 */
    uint64_t q = n - 1;
    if (q == 2) return 2;           /* handle even 2 */
    if ((q & 1) == 0) q--;          /* make q odd */
    for (; q >= 3; q -= 2) {
        if (is_prime_uint64(q)) return q;
        if (q <= 3) break;
    }
    return 0;                       /* safety */
}

/* ---------- memoised sequence generation ---------- */

static seq_t *term_list   = NULL;    /* stored values */
static size_t term_cnt    = 0;       /* number of terms stored */
static size_t term_cap    = 0;       /* allocated capacity */
static uint64_t last_k    = 0;       /* last integer examined */
static unsigned long long v2_fact = 0; /* v2(k!) for k = last_k */

/* enlarge array when needed */
static void ensure_capacity(size_t needed) {
    if (term_cap < needed) {
        size_t new_cap = term_cap ? term_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        term_list = (seq_t *)realloc(term_list, new_cap * sizeof(seq_t));
        term_cap = new_cap;
    }
}

/* sequence entry:
 * a(n) is the nth term (n≥1).  Calls to a(n) may be in any order.
 */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined; avoid crash */

    while (term_cnt < (size_t)n) {
        ++last_k;
        /* update exponent of 2 in (last_k)! */
        unsigned v2_k = (last_k == 0) ? 0 : __builtin_ctzll(last_k);
        v2_fact += v2_k;

        /* largest prime ≤ last_k */
        uint64_t prime_leq = prevprime_uint64(last_k + 1);

        /* only consider valid primes (≠0) */
        if (prime_leq != 0 && v2_fact == prime_leq) {
            ensure_capacity(term_cnt + 1);
            term_list[term_cnt++] = (seq_t)last_k;
        }
    }
    return term_list[n - 1];
}
