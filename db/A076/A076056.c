/*
 * A076056  Primes which when read backwards are composite numbers.
 * https://oeis.org/A076056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 5492, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A076056:
   primes whose decimal reversal is a composite number. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* modular multiplication without overflow (using addition) */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod) {
    uint64_t res = 0;
    a %= mod;
    while (b) {
        if (b & 1ULL) {
            res += a;
            if (res >= mod) res -= mod;
        }
        a <<= 1;
        if (a >= mod) a -= mod;
        b >>= 1;
    }
    return res;
}

/* modular exponentiation */
static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller–Rabin test for 64‑bit integers */
static bool is_prime_mr(uint64_t n) {
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); i++) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1; s++;
    }
    /* witnesses that guarantee correctness for 64‑bit */
    static const uint64_t R_64[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(R_64)/sizeof(R_64[0]); i++) {
        uint64_t a = R_64[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; r++) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* decimal digit reversal (uint64_t only) */
static uint64_t rev_uint64(uint64_t v) {
    uint64_t rev = 0;
    while (v) {
        rev = rev * 10 + v % 10;
        v /= 10;
    }
    return rev;
}

/* sequence generator */
seq_t a(idx_t n) {
    /* static memoization state */
    static uint64_t* prime_list = NULL;
    static size_t prime_cnt = 0, prime_cap = 0;
    static seq_t* cache = NULL;
    static size_t cache_cnt = 0, cache_cap = 0;
    static uint64_t curr = 2;     /* next candidate for prime */
    static bool init = false;

    if (!init) {                 /* one‑time initialization */
        prime_cap = 8;
        prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        init = true;
    }

    while (cache_cnt < (size_t)n) {
        /* generate next prime */
        uint64_t p;
        if (curr == 2) {
            p = 2;
            curr = 3;
        } else {
            /* skip non‑prime candidates */
            while (1) {
                bool is_p = true;
                uint64_t limit = (uint64_t)sqrt((double)curr);
                for (size_t i = 0; i < prime_cnt; i++) {
                    uint64_t pr = prime_list[i];
                    if (pr > limit) break;
                    if (curr % pr == 0) {
                        is_p = false;
                        break;
                    }
                }
                if (is_p) {
                    p = curr;
                    if (prime_cnt == prime_cap) {
                        prime_cap *= 2;
                        prime_list = (uint64_t*)realloc(prime_list,
                                prime_cap * sizeof(uint64_t));
                    }
                    prime_list[prime_cnt++] = p;
                    curr += 2;
                    break;
                }
                curr += 2;
            }
        }

        /* test whether the reversal is composite */
        uint64_t rev = rev_uint64(p);
        if (rev <= 1 || !is_prime_mr(rev)) {
            if (cache_cnt == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache,
                        cache_cap * sizeof(seq_t));
            }
            cache[cache_cnt++] = (seq_t)p;
        }
    }
    return cache[n - 1];
}
