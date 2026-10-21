/*
 * A224862  Numbers n such that 90*n + 71 and 90*n + 73 are twin primes.
 * https://oeis.org/A224862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that 90*k+71 and 90*k+73 are twin
 *            primes. The sequence is indexed from 1 (a(1)=0). The function
 *            uses a deterministic Miller–Rabin test for 64‑bit numbers and
 *            lazy expansion of the result array
 * Range:     u8 n <= 39, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((unsigned __int128)a * b % mod);
}

static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1;
    while (exp) {
        if (exp & 1) res = mul_mod(res, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return res;
}

static bool miller_rabin_witness(uint64_t a, uint64_t n, uint64_t d, uint64_t s) {
    uint64_t x = pow_mod_uint64(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (uint64_t r = 1; r < s; ++r) {
        x = mul_mod(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

/* Deterministic Miller–Rabin for 64‑bit integers.
   Uses the seven bases that are correct for all n < 2^64. */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    /* small primes that do not appear in the progression 90*k+71 or 90*k+73
       (they are also used here to quickly discard obvious composites) */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        if (!miller_rabin_witness(a, n, d, s)) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Lazy storage of discovered terms */
    static seq_t *terms = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static idx_t next_trial = 0;        /* the next k to test */

    if (n == 0) return 0;  /* undefined but harmless for safety */

    /* Expand until we have at least n terms */
    while (len < n) {
        uint64_t v1 = 90ULL * (uint64_t)next_trial + 71ULL;
        uint64_t v2 = v1 + 2ULL;  /* 90*n+73 */
        if (is_prime_uint64(v1) && is_prime_uint64(v2)) {
            if (len == cap) {
                cap = (cap == 0) ? 16 : cap * 2;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = (seq_t)next_trial;
        }
        ++next_trial;
    }
    return terms[n - 1];
}
