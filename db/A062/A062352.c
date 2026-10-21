/*
 * A062352  Palindromic primes with strictly decreasing digits up to the
 *          middle and then strictly increasing.
 * https://oeis.org/A062352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   generate palindromic primes whose digits strictly decrease to
 *            the middle and then strictly increase
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 67, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* deterministic Miller–Rabin primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t xa = 1, pa = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) xa = (xa * pa) % n;
            pa = (pa * pa) % n;
            exp >>= 1;
        }
        if (xa == 1 || xa == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            xa = (xa * xa) % n;
            if (xa == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* comparison function for uint64_t for qsort */
static int cmp_uint64(const void *p1, const void *p2) {
    uint64_t a = *(const uint64_t*)p1;
    uint64_t b = *(const uint64_t*)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* static storage for the sequence */
static seq_t terms[256];
static size_t terms_count = 0;
static bool initialized = false;

static const uint64_t LIMIT_U64 = 97654321012345679ULL;

/* initialize the term array */
static void init_terms(void) {
    uint64_t tmp[4096];
    size_t tmpcnt = 0;

    for (int mask = 1; mask < (1 << 10); ++mask) {
        /* digits present in the mask, ascending order */
        int digits[10], dcnt = 0;
        for (int d = 0; d < 10; ++d)
            if (mask & (1 << d)) digits[dcnt++] = d;
        if (digits[dcnt - 1] == 0) continue;   /* leading zero not allowed */

        /* descending order of digits (left half of the palindrome) */
        int rev[10];
        for (int i = 0; i < dcnt; ++i) rev[i] = digits[dcnt - 1 - i];

        /* even length palindrome */
        uint64_t val = 0;
        bool overflow = false;
        for (int i = 0; i < dcnt && !overflow; ++i) {
            if (val > (UINT64_MAX - rev[i]) / 10) { overflow = true; break; }
            val = val * 10 + rev[i];
        }
        if (!overflow) {
            for (int i = dcnt - 1; i >= 0 && !overflow; --i) {
                if (val > (UINT64_MAX - rev[i]) / 10) { overflow = true; break; }
                val = val * 10 + rev[i];
            }
        }
        if (!overflow && val <= LIMIT_U64 && is_prime_uint64(val))
            tmp[tmpcnt++] = val;

        /* odd length palindrome */
        val = 0;
        overflow = false;
        for (int i = 0; i < dcnt && !overflow; ++i) {
            if (val > (UINT64_MAX - rev[i]) / 10) { overflow = true; break; }
            val = val * 10 + rev[i];
        }
        if (!overflow) {
            for (int i = dcnt - 2; i >= 0 && !overflow; --i) {
                if (val > (UINT64_MAX - rev[i]) / 10) { overflow = true; break; }
                val = val * 10 + rev[i];
            }
        }
        if (!overflow && val <= LIMIT_U64 && is_prime_uint64(val))
            tmp[tmpcnt++] = val;
    }

    /* sort candidates */
    qsort(tmp, tmpcnt, sizeof(uint64_t), cmp_uint64);

    /* copy unique values to terms array */
    size_t idx = 0;
    for (size_t i = 0; i < tmpcnt; ++i) {
        if (i == 0 || tmp[i] != tmp[i-1]) {
            if (idx < 256) {
                terms[idx++] = (seq_t)tmp[i];
            }
        }
    }
    terms_count = idx;
}

/* public function required by the harness */
seq_t a(idx_t n) {
    if (!initialized) {
        init_terms();
        initialized = true;
    }
    if (n == 0 || (idx_t)terms_count < n) return 0;
    return terms[n - 1];
}
