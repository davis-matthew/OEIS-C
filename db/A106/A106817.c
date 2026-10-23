/*
 * A106817  Smallest prime of the set of four consecutive primes whose sum of
 *          digits is a set of four distinct primes.
 * https://oeis.org/A106817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest prime of each block of four consecutive
 *            primes whose digit sums are four distinct primes. The
 *            implementation searches primes sequentially and collects
 *            qualifying blocks, memoising results for repeated calls
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_small_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_small_prime.h"

static inline uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1ULL)
            result = mod_mul_uint64(result, base, mod);
        base = mod_mul_uint64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }

    const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont_outer = false;
        for (int r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) {
                cont_outer = true;
                break;
            }
        }
        if (cont_outer) continue;
        return false;
    }
    return true;
}

static inline uint64_t next_prime(uint64_t prev) {
    if (prev < 2) return 2;
    uint64_t cand = prev + 1;
    if (cand > 2 && (cand & 1ULL) == 0ULL) cand++;  // skip even numbers
    while (!is_prime_uint64(cand)) {
        cand += 2;
    }
    return cand;
}

static inline int digit_sum_uint64(uint64_t x) {
    int s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n) {
    /* Static state for memoised computation */
    static seq_t *terms = NULL;
    static idx_t terms_size = 0;
    static idx_t terms_capacity = 0;
    static uint64_t last_prime = 1;
    static uint64_t window[4];
    static int win_pos = 0;
    static int win_len = 0;

    /* Ensure enough terms are generated */
    while (terms_size < n) {
        uint64_t prime = next_prime(last_prime);
        last_prime = prime;
        window[win_pos] = prime;
        win_pos = (win_pos + 1) % 4;
        if (win_len < 4) ++win_len;

        if (win_len == 4) {
            uint64_t block[4];
            for (int i = 0; i < 4; ++i) {
                block[i] = window[(win_pos + i) % 4];
            }

            int sum[4];
            bool ok = true;
            for (int i = 0; i < 4; ++i) {
                sum[i] = digit_sum_uint64(block[i]);
                if (!is_small_prime(sum[i])) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                for (int i = 0; i < 4 && ok; ++i) {
                    for (int j = i + 1; j < 4; ++j) {
                        if (sum[i] == sum[j]) {
                            ok = false;
                            break;
                        }
                    }
                }
            }
            if (ok) {
                if (terms_size + 1 > terms_capacity) {
                    terms_capacity = terms_capacity == 0 ? 8 : terms_capacity << 1;
                    terms = (seq_t *)realloc(terms, terms_capacity * sizeof(seq_t));
                }
                terms[terms_size] = (seq_t)block[0];
                ++terms_size;
            }
        }
    }
    return terms[n - 1];
}
